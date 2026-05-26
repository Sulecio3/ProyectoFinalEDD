# Proyecto Final EDD - Generador de imagenes por capas

Proyecto realizado en C++ para la clase de Estructura de Datos I.

## Estructuras utilizadas

- Arbol binario de busqueda para capas.
- Matriz dispersa para los pixeles de cada capa.
- Lista circular doblemente enlazada para imagenes.
- Arbol binario para usuarios.
- Lista simple para las imagenes asignadas a cada usuario.

## Archivos principales

- `main.cpp`: menu principal del sistema.
- `capas.cpp`: carga de capas, ABB de capas, matriz dispersa y reportes de capas.
- `imagenes.cpp`: lista circular doble de imagenes, carga de imagenes, generacion y reportes.
- `usuarios.cpp`: arbol de usuarios, carga de usuarios, CRUD y generacion por usuario.

## Archivos de entrada

- `capas.cap`
- `imagenes.im`
- `usuarios.usr`

## Orden de carga recomendado

1. Cargar capas.
2. Cargar imagenes.
3. Cargar usuarios.

## Graphviz

El programa genera archivos `.dot` y `.png` usando Graphviz.
Si no se generan los `.png`, revisar que Graphviz este instalado y agregado al PATH.

## Pruebas rapidas

En el menu se puede probar:

- Opcion 1: cargar `capas.cap`.
- Opcion 4: cargar `imagenes.im`.
- Opcion 7: cargar `usuarios.usr`.
- Opcion 10: generar imagen por capa.
- Opcion 11: generar imagen por lista de imagen.
- Opcion 12: generar imagen por usuario.
- Opcion 13: graficar arbol de capas.
- Opcion 14: graficar lista circular de imagenes.
- Opcion 15: graficar arbol de usuarios.
- Opcion 16: graficar imagen y arbol de capas.
- Opcion 22: generar imagen por recorrido limitado.
- Opcion 23: graficar capa como matriz dispersa.
