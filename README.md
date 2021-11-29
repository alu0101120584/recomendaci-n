# Sistema de recomendaciones

### Autor: Juan Eduardo Reyes Pérez
### Asignatura: GEstión del Conocimiento en las organizaciones
### Fecha: 28/11/2021


Para ejecutar el programa de forma correcta se deben pasar los siguientes argumentos por línea de comandos: fichero de entrada, fichero de salida, métrica a utilizar, número de vecinos, predicción a utilizar.
En el fichero que seleccionemos como entrada se debe incluir una matriz de prueba con el siguiente formato:

~~~ C++
5 3 4 4 -
3 1 2 3 3
4 3 4 3 5
3 3 1 5 4
1 5 5 2 1
~~~

Un ejemplo sería: **./main input.txt output.txt pears 3 doble**