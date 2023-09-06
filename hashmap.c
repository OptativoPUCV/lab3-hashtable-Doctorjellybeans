#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
  if (map == NULL || key == NULL){
    return;
  }
  int indice = hash(key, map->capacity);

  while (map->buckets[indice] != NULL && map->buckets[indice]->key != NULL){
    if (strcmp(key, map->buckets[indice]->key) == 0) {
      return;
    }
    indice = (indice + 1) % map->capacity;
  }
  map->buckets[indice] = createPair(key, value);
  map->current = indice;
  map->size++;
}


void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)

  if (map == NULL){
    return;
  }
  
  Pair **old_buckets = map->buckets;
  
  long nuevaCapacidad = map->capacity * 2;
  map->buckets = (Pair**)malloc(nuevaCapacidad * sizeof(Pair*));
  
  map->size = 0;

  for (long i = 0; i < nuevaCapacidad; i++){
    map->buckets[i] = NULL;
  }

  for (long i = 0; i < map->capacity; i++){
    
    if (old_buckets[i] != NULL && old_buckets[i]->key != NULL){
      long indice = hash(old_buckets[i]->key, nuevaCapacidad);
      insertMap(map, old_buckets[i]->key, old_buckets[i]->value);
    }
  
  }

  free(old_buckets);
  map->capacity = nuevaCapacidad;
}


HashMap * createMap(long capacity) {
  HashMap* mapa = malloc(sizeof(HashMap));
  if (mapa != NULL){
    // Inicializar arreglo en nulo
    mapa->buckets = (Pair**)calloc(capacity, sizeof(Pair*));
    if (mapa->buckets != NULL){
      mapa->size = 0;
      mapa->capacity = capacity;
      mapa->current = -1;
    } else {
      free(mapa);
      mapa = NULL;
    }
  }

  return mapa;
}

void eraseMap(HashMap * map,  char * key) {    
  if (map == NULL || key == NULL){
    return;
  }

  Pair *parejaBorrada = searchMap(map, key);

  if (parejaBorrada != NULL){
    parejaBorrada->key = NULL;
    map->current = hash(key,map->capacity);
    map->size--;
  }

}

Pair * searchMap(HashMap * map,  char * key) {   
  if (map == NULL || key == NULL){
    return NULL;
  }

  int indice = hash(key, map->capacity);
  int i = 0;
  while (i < map->capacity){
    if (map->buckets[indice] != NULL && map->buckets[indice]->key != NULL && strcmp(map->buckets[indice]->key,key) == 0){
      map->current = indice;
      return map->buckets[indice];
    }
    indice = (indice + 1) % map->capacity;
    i++;
  }

  return NULL;
}

Pair * firstMap(HashMap * map) {
  if (map == NULL){
    return NULL;
  }

  int indice = 0;
  
  while (map->buckets[indice] == NULL || map->buckets[indice]->key == NULL){
    indice++;
  }
  
  map->current = indice;
  return map->buckets[indice];
}

Pair * nextMap(HashMap * map) {
  if (map == NULL){
    return NULL;
  }
  
  int indice = map->current + 1;
  while (indice < map->capacity){
    if (map->buckets[indice] != NULL && map->buckets[indice]->key != NULL){
      map->current = indice;
      return map->buckets[indice];
    }
    indice++;
  }
  return NULL;
}
