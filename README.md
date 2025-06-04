# Laboratorio 4: "Red" - Grupo 29 - FaMAF (2025)
> ### Integrantes
> - Juan Cruz Hermosilla Artico
> - Christian Morales
> - Gaspar Sáenz Valiente
> - Fernando Cabrera Luque

## Abstract
En este trabajo se presenta el análisis de una red de ocho routers dispuesta en topología de anillo, simulada con **OMNeT++**, como parte del Laboratorio 4 de la cátedra de Redes y Sistemas Distribuidos de la UNC. Se realizan dos escenarios con distintos patrones de tráfico: uno con tráfico controlado donde solo dos de los nodos generan paquetes hacia uno de los nodos y otro donde hay múltiples nodos emitiendo paquetes hacia el mismo nodo. De la observación de estos escenarios se recolecta información relevante para el análisis del rendimiento de la red. Luego se realiza una labor de diseño en la que buscamos implementar un algoritmo que permita elegir el mejor trayecto para el envío de los paquetes y mejorar así la eficiencia de la red.

## Introducción
La capa de red es la responsable de encaminar paquetes entre routers y encontrar el trayecto más eficiente desde un nodo emisor y un nodo receptor. En este trabajo realizaremos el analisis de una red de ocho nodos (identificados de 0..7) enlazadas con topología de anillo donde simularemos tráfico dirigido hacia el nodo 5. 
El análisis se divide en dos etapas. En la primera los paquetes se envían en sentido horario (siempre hacia la misma interfaz) hasta llegar a destino, sin considerar distancias. En la segunda, se realiza el diseño de un algoritmo dinámico en la que se implementará una estrategia en la que se busque el camino más corto para mejorar la eficiencia de la red.
Dentro de cada etapa, además se dividirán en dos casos en lo que se varía el tráfico. Tendremos un caso más controlado donde solo dos de los nodos generarán tráfico, y uno más cargado donde todos los nodos, menos el receptor, estarán generando y enviando paquetes. 
Los principales datos que iremos recolectando para realizar el análisis serán: la cantidad de saltos que realizan los paquetes hasta llegar a destino, el retraso y la entrega de los mismos, y el uso de los recursos de los nodos. 

## Glosario de abreviaturas
Antes de comenzar con el desarrollo del informe, consideramos relevante explicitar aquellas abreviaturas que se utilizan a lo largo del mismo en pos de mejorar la comprensión para el lector y disipar potenciales confusiones o malentendidos. 
- **s**  → segundos
- **ms** → milisegundo
- **b**  → bit
- **B**  → bytes (8 b)
- **Mb** → megabit (1.000.000 b)

## Modelo y entorno
En este apartado describiremos el modelo de red y el entorno de simulación empleado. Para realizar las simulaciones utilizaremos **OMNeT++**, una biblioteca y marco de simulación en C++. Dichas simulaciones, luego de cada ejecución, se genera información acerca de saltos por paquetes, de uso del *buffer*, de los paquetes recibidos, de el retraso de entrega, y de otros datos de interés. 
Para ser más precisos realizaremos varias simulaciones manteniendo los mismos parámetros. Esto nos permite observar el comportamiento variable que puede observarse en estos sistemas.
Como mencionamos en el apartado anterior, la red contará con una topología en forma de anillo con ocho nodos, identificados por numeros de 0..7. Cada nodo cuenta con: dos capas de enlace (`lnk[0]` y `lnk[1]`, una por vecino), una capa de red (`net`) y una capa de aplicación (`app`). La capa de aplicación se encarga de generar el tráfico y la capa de enlace implementa el buffer. Los paquetes serán enviados por las capas de enlace desde que son generados hasta llegar a destino, la eleccion de por cual `lnk` se envía el paquete determina si el movimiento será en modo horario o antihorario dentro de la topología utilizada.

![nodos y topología](/images/icon/anillo.png)

Para las simulaciones de ambos casos, los parametros que se compartiran serán:
* **Tiempo de simulación**: de 200 s.
* **Tamaño de paquete** : `packetByteSize = 125000` (equivalente a 1 Mb).
* **Nodo de destino**: el nodo 5.

## Análisis - primera parte
### Caso 1 y Caso 2

En esta sección haremos un análisis compartivo de ambos casos, centrandonos en el retardo end-to-end en el nodo 5, donde los paquetes son recibidos, y en la ocupación de *buffers* de los nodos. En ambos casos utilizaremos el mismo intervalo de generación de paquetes para facilitar la comparación.

![delay ambos casos](/images/graph/delay-primera-parte.png)

En este gráfico observamos la evolución del *delay* de ambos casos. La curva azul se corresponde al Caso 1 (tráfico desde los nodos 0 y 2 hacia el nodo 5), mientras que la curva naranja muestra el comportamiento del Caso 2 (tráfico generado desde todos los nodos hacia el nodo 5). En los primeros segundos de simulación, ambos casos mantinenen valores similares, ya que la cantidad de paquetes en circulación aún es baja y no se forman colas significativas.
Sin embargo, a partir de los 15-20 s se comienza a marcar una mayor diferencia: el Caso 1 mantiene un crecimiento casi lineal, mientras que en el Caso 2 se muestra un comportamiento más abrupto y fluctuante. Al final de la simulación el Caso 2 supera los 175 ms de *delay*, casi duplicando el tiempo del Caso 1, que apenas pasa la barrera de los 100 ms.

<!-- ![buffer caso 1](/images/graph/buffer-caso1-parte1.png) -->

![buffer caso 2](/images/graph/buffer-caso2-parte1.png)
Para entender esta diferencia, observaremos el uso de los *buffers* del Caso 2. El gráfico nos muestra como casi todos los nodos acumulan paquetes de manera significativa, algunos superando los 200. Esto indica que el tráfico esta saturando a la mayoría de nodos. La principal excepción es el nodo 6, el cual tiene la ubicación más cercana al nodo 5 en el sentido en que los paquetes se están moviendo por la red.

Recordemos que en ambos casos la ruta que siguen los paquetes en el anillo es en sentido horario, sin importar si es o no la ruta más eficiente para que circulen. Al ser el objetivo de todos los paquetes el ser recibidos por el nodo 5, los nodos más alejados del circuito, o sea los más próximos en sentido horario (siendo los nodos 4, 3 y 2) van a generar un tráfico innecesario en el resto de nodos que median con el mismo. Una mejora a este sistema es lograr que cada nodo "sepa" hacia que sentido enviar el paquete buscando la ruta más corta a destino. En proximas secciones propondremos una posible solución a este problema. 

<!-- ![buffers casos 1 y 2](/images/graph/buffer-parte1-nodo6.png) -->
### Equilibrio en el Caso 2
Luego de probar intervalos de generación, observamos que la red alcanza un comportamiento más estable en cuanto al retardo de entrega cuando el `interArrivalTime`(variable que determina la generación de paquetes) se reduce a 0.1 s o tiempos inferiores. Esto, de todas formas, no mejora el *delay* final de la simulación. 
A continuación se muestra el gráfico que muestra la evolución del delay en este caso:
![delay-0.1-caso2](/images/graph/delay-caso2-0.1.png)

## Tarea de diseño

## Análisis - segunda parte

## Conclusión
