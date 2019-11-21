#define _POSIX_C_SOURCE 200809L /* Para el strdup */
#include "hash.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define CAPACIDAD_INICIAL TABLA_PRIMOS[0];
const size_t TABLA_PRIMOS[];
const float FACTOR_DE_CARGA_MAXIMO = 0.6f;
const float FACTOR_DE_CARGA_MINIMO = 0.2f;
/* Lo estados de cada balde */
enum ESTADO {
	VACIO = 0,
	BORRADO = 1,
	OCUPADO = 2,
};
const uint32_t MUMUR_HASH_SEED = 17;

typedef struct bucket {
	void* dato;
	char* clave;
	enum ESTADO estado;
} bucket_t;

struct hash {
	bucket_t* baldes;
	size_t cantidad;
	size_t capacidad;
	size_t borrados;
	size_t indice_tabla;
	hash_destruir_dato_t destructor;
};

typedef bucket_t* (*predicado_busqueda_t)(bucket_t*, const char*, bool*);

uint32_t murmurhash (const char *key, uint32_t len, uint32_t seed);

size_t hashear_string(const char* clave) {
	return murmurhash(clave, (uint32_t)strlen(clave), MUMUR_HASH_SEED);
}

bucket_t* buscar_balde(const hash_t* hash, const char* clave, predicado_busqueda_t predicado) {
	size_t inicio = hashear_string(clave) % hash->capacidad;
	bucket_t* out_bucket = NULL;
	bool encontrado = false;
	for (size_t i = 0; i < hash->capacidad && !encontrado; ++i) {
		bucket_t* bucket = &hash->baldes[(inicio + i) % hash->capacidad];
		out_bucket = predicado(bucket, clave, &encontrado);
	}
	return out_bucket;
}

bucket_t* balde_vacio(bucket_t* bucket, const char* clave, bool* encontrado) {
	if (bucket->estado == VACIO) {
		*encontrado = true;
		return bucket;
	}
	return NULL;
}

bucket_t* balde_existente(bucket_t* bucket, const char* clave, bool* encontrado) {
	if (bucket->estado == OCUPADO && strcmp(bucket->clave, clave) == 0) {
		*encontrado = true;
		return bucket;
	}
	else if (bucket->estado == VACIO) {
		*encontrado = true;
		return NULL;
	}
	return NULL;
}

bucket_t* buscar_balde_existente(const hash_t* hash, const char* clave) {
	return buscar_balde(hash, clave, &balde_existente);
}

bucket_t* buscar_balde_vacio(const hash_t* hash, const char* clave) {
	return buscar_balde(hash, clave, &balde_vacio);
}

void inicializar_baldes(bucket_t* baldes, size_t capacidad) {
	for (int i = 0; i < capacidad; ++i) {
		baldes[i].clave = NULL;
		baldes[i].dato = NULL;
		baldes[i].estado = VACIO;
	}
}

float calcular_carga_actual(const hash_t* hash) {
	return ((float)hash->borrados + (float)hash->cantidad) / (float)hash->capacidad;
}

bool necesita_agrandar(const hash_t* hash) {
	float carga_actual = ((float)hash->borrados + (float)hash->cantidad) / (float)hash->capacidad;
	return carga_actual >= FACTOR_DE_CARGA_MAXIMO;
}

bool necesita_achicar(const hash_t* hash) {
	float carga_sin_borrados = ((float)hash->cantidad) / (float)hash->capacidad;
	return carga_sin_borrados <= FACTOR_DE_CARGA_MINIMO && hash->capacidad > CAPACIDAD_INICIAL;
}

bucket_t* crear_baldes(hash_t* hash, size_t capacidad) {
	bucket_t* baldes = malloc(sizeof(bucket_t) * capacidad);
	if (baldes)
		inicializar_baldes(baldes, capacidad);
	return baldes;
}

bool redimensionar_hash(hash_t* hash, size_t new_capacity) {
	bucket_t* new_buckets = crear_baldes(hash, new_capacity);
	if (!new_buckets) return false;

	size_t old_capacity = hash->capacidad;
	bucket_t* old_buckets = hash->baldes;
	/* Setiamos los baldes y actualizamos los contadores */
	hash->capacidad = new_capacity;
	hash->baldes = new_buckets;
	hash->cantidad = 0;
	hash->borrados = 0;
	for (size_t i = 0; i < old_capacity; ++i) {
		/* Solo copiamos los buckets que tenian algun elemento*/
		if (old_buckets[i].estado != OCUPADO) continue;
		hash_guardar(hash, old_buckets[i].clave, old_buckets[i].dato);
		/* Como guardamos una copia del string le hacemos un free al antiguo */
		free(old_buckets[i].clave);
	}
	free(old_buckets);
	return true;
}

bool achicar_hash(hash_t* hash) {
	return redimensionar_hash(hash, TABLA_PRIMOS[--hash->indice_tabla]);
}

bool agrandar_hash(hash_t* hash) {
	return redimensionar_hash(hash, TABLA_PRIMOS[++hash->indice_tabla]);
}

bool reemplazar_dato(const hash_t* hash, bucket_t* bucket, void* dato) {
	if (hash->destructor)
		hash->destructor(bucket->dato);
	bucket->dato = dato;
	return true;
}

bool guardar_dato(hash_t* hash, bucket_t* balde_vacio, const char* clave, void* dato) {
	balde_vacio->estado = OCUPADO;
	balde_vacio->dato = dato;
	balde_vacio->clave = strdup(clave);
	hash->cantidad++;
	return true;
}

void* delete_bucket(hash_t* hash, bucket_t* bucket) {
	void* dato = bucket->dato;
	char* clave = bucket->clave;
	bucket->dato = NULL;
	bucket->estado = BORRADO;
	bucket->clave = NULL;
	/* Al borrar un elemento tenemos que hacerle un free a su clave ya que es una copia del original */
	free(clave);
	hash->cantidad--;
	hash->borrados++;
	return dato;
}

/* PRIMITIVAS DEL HASH */

hash_t* hash_crear(hash_destruir_dato_t destruir_dato) {
	hash_t* hash = malloc(sizeof(hash_t));
	if (!hash) return NULL;
	hash->capacidad = CAPACIDAD_INICIAL;
	hash->cantidad = 0;
	hash->borrados = 0;
	hash->indice_tabla = 0;
	hash->baldes = crear_baldes(hash, hash->capacidad);
	hash->destructor = destruir_dato;
	if (!hash->baldes) {
		free(hash);
		return NULL;
	}
	return hash;
}

bool hash_guardar(hash_t* hash, const char* clave, void* dato) {
	if (necesita_agrandar(hash)) {
		if (!agrandar_hash(hash)) return false;
	}
	bucket_t* bucket = buscar_balde_existente(hash, clave);
	if (bucket) return reemplazar_dato(hash, bucket, dato);

	bucket_t* balde_vacio = buscar_balde_vacio(hash, clave);
	if (balde_vacio) return guardar_dato(hash, balde_vacio, clave, dato);

	return false;
}

void* hash_borrar(hash_t* hash, const char* clave) {
	bucket_t* bucket = buscar_balde_existente(hash, clave);
	if (!bucket) return NULL;
	void* result = delete_bucket(hash, bucket);
	if (necesita_achicar(hash)) achicar_hash(hash);
	return result;
}

void* hash_obtener(const hash_t* hash, const char* clave) {
	bucket_t* bucket = buscar_balde_existente(hash, clave);
	return bucket != NULL ? bucket->dato : NULL;
}

bool hash_pertenece(const hash_t* hash, const char* clave) {
	bucket_t* bucket = buscar_balde_existente(hash, clave);
	return bucket != NULL;
}

size_t hash_cantidad(const hash_t* hash) {
	return hash->cantidad;
}

void hash_destruir(hash_t* hash) {
	for (size_t i = 0; i < hash->capacidad; ++i) {
		if (hash->destructor && hash->baldes[i].estado == OCUPADO)
			hash->destructor(hash->baldes[i].dato);
		/* Todos los buckets que estan ocupados tienen una copia de la clave, la cual hay q hacerle free */
		if (hash->baldes[i].clave)
			free(hash->baldes[i].clave);
	}
	free(hash->baldes);
	free(hash);
}

/* Iterador del hash */

struct hash_iter {
	const hash_t* hash;
	size_t actual;
};

void ir_al_siguiente(hash_iter_t* iter) {
	bucket_t* buckets = iter->hash->baldes;
	while (!hash_iter_al_final(iter) && buckets[iter->actual].estado != OCUPADO)
		iter->actual++;
}

hash_iter_t* hash_iter_crear(const hash_t* hash) {
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	if (!iter) return NULL;
	iter->hash = hash;
	iter->actual = 0;
	/* Ponemos el indice al inicio del hash y hacemos que el iterador avanza hasta el primero elemento. Si esta vacio va a llegar al final y terminar */
	ir_al_siguiente(iter);
	return iter;
}

bool hash_iter_avanzar(hash_iter_t* iter) {
	if (hash_iter_al_final(iter)) return false;
	iter->actual++;
	ir_al_siguiente(iter);
	return !hash_iter_al_final(iter);
}

const char* hash_iter_ver_actual(const hash_iter_t* iter) {
	return !hash_iter_al_final(iter) ? iter->hash->baldes[iter->actual].clave : NULL;
}

bool hash_iter_al_final(const hash_iter_t* iter) {
	return iter->actual == iter->hash->capacidad;
}

void hash_iter_destruir(hash_iter_t* iter) {
	free(iter);
}

/* Funcion de hash, saque el nombre de stackoverflow y despues busque una implementacion en C */
uint32_t murmurhash (const char *key, uint32_t len, uint32_t seed) {
  uint32_t c1 = 0xcc9e2d51;
  uint32_t c2 = 0x1b873593;
  uint32_t r1 = 15;
  uint32_t r2 = 13;
  uint32_t m = 5;
  uint32_t n = 0xe6546b64;
  uint32_t h = 0;
  uint32_t k = 0;
  uint8_t *d = (uint8_t *) key; // 32 bit extract from `key'
  const uint32_t *chunks = NULL;
  const uint8_t *tail = NULL; // tail - last 8 bytes
  int i = 0;
  int l = (int) len / 4; // chunk length

  h = seed;

  chunks = (const uint32_t *) (d + l * 4); // body
  tail = (const uint8_t *) (d + l * 4); // last 8 byte chunk of `key'

  // for each 4 byte chunk of `key'
  for (i = -l; i != 0; ++i) {
    // next 4 byte chunk of `key'
    k = chunks[i];

    // encode next 4 byte chunk of `key'
    k *= c1;
    k = (k << r1) | (k >> (32 - r1));
    k *= c2;

    // append to hash
    h ^= k;
    h = (h << r2) | (h >> (32 - r2));
    h = h * m + n;
  }

  k = 0;

  // remainder
  switch (len & 3) { // `len % 4'
    case 3: k ^= (uint32_t)(tail[2] << 16);
    case 2: k ^= (uint32_t)(tail[1] << 8);

    case 1:
      k ^= tail[0];
      k *= c1;
      k = (k << r1) | (k >> (32 - r1));
      k *= c2;
      h ^= k;
  }

  h ^= len;

  h ^= (h >> 16);
  h *= 0x85ebca6b;
  h ^= (h >> 13);
  h *= 0xc2b2ae35;
  h ^= (h >> 16);

  return h;
}

const size_t TABLA_PRIMOS[] = {
	13,
	29,
	53,
	97,
	193,
	389,
	769,
	1543,
	3079,
	6151,
	12289,
	24593,
	49157,
	98317,
	196613,
	393241,
	786433,
	1572869,
	3145739,
	6291469,
	12582917,
	25165843,
	50331653,
	100663319,
	201326611,
	402653189,
	805306457,
	1610612741
};