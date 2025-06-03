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
- **s** → segundos
- **b** → bit
- **B** → bytes (8 b)
- **Mb** -> megabit (1.000.000 b)

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

![delay ambos casos](/images/graph/delay-primera-parte.png)

![buffer caso 1](/images/graph/buffer-caso1-parte1.png)

![buffer caso 2](/images/graph/buffer-caso2-parte1.png)

![buffers casos 1 y 2](/images/graph/buffer-parte1-nodo6.png)

## Tarea de diseño

## Análisis - segunda parte

## Conclusión
