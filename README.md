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
Para mejorar la eficiencia de la red, se implementó un algoritmo de enrutamiento dinámico por **Vector de Distancias (DV)** en el módulo `Net` de cada nodo. Este algoritmo permite a los nodos descubrir la ruta más corta (en términos de saltos) hacia todos los demás nodos de la red. La implementación se basa en los siguientes componentes:

1.  **Descubrimiento de Vecinos:**
    *   Al iniciar la simulación, cada nodo envía mensajes `Hello` (definidos en `hello.txt` y `hello_m.txt`) por todas sus interfaces de red.
    *   Al recibir un mensaje `Hello` de un nodo vecino, se registra a este vecino en la tabla de enrutamiento (`routingTable`) con un costo de 1 (un salto directo) y se almacena la interfaz de llegada en `gateTable`. Esto permite al nodo saber por dónde enviar paquetes destinados a ese vecino directo.

2.  **Intercambio de Vectores de Distancia:**
    *   Cada nodo mantiene una tabla de enrutamiento (`routingTable`) que almacena, para cada destino conocido, el costo (número de saltos) y el `nextNode` (el vecino al que se debe enviar el paquete para alcanzar dicho destino).
    *   Cuando la tabla de un nodo cambia (o periódicamente, aunque en esta implementación es principalmente por cambios), el nodo construye un mensaje `DistanceVectorMsg` (definido en `distanceVector.txt` y `distanceVector_m.txt`). Este mensaje contiene el vector de distancias del nodo emisor, que es una lista de pares `(destino, costo)`.
    *   El `DistanceVectorMsg` se envía a todos los vecinos directos.

3.  **Actualización de la Tabla de Enrutamiento (Algoritmo de Bellman-Ford):**
    *   Cuando un nodo `U` recibe un `DistanceVectorMsg` de un vecino `V`, procesa cada entrada `(D, cost(V,D))` del vector recibido.
    *   Para cada destino `D`, `U` calcula un posible nuevo costo hacia `D` a través de `V` como: `new_cost(U,D) = cost(U,V) + cost(V,D) = 1 + cost(V,D)`.
    *   Si `D` no está en la tabla de `U`, o si `new_cost(U,D)` es menor que el costo actual de `U` a `D`, o si la ruta actual de `U` a `D` es a través de `V` y el costo `cost(V,D)` ha cambiado, entonces `U` actualiza su entrada para `D`: el costo se establece en `new_cost(U,D)` y el `nextNode` para alcanzar `D` se establece en `V`. La `gateTable` se actualiza para reflejar la interfaz de salida hacia `V`.
    *   Esta lógica se implementa en la función `Net::updateTable`.

4.  **Manejo y Reenvío de Paquetes de Datos:**
    *   Cuando el módulo `Net` recibe un paquete de datos (`Packet`):
        *   Si el nodo actual es el destino del paquete, este se entrega a la capa de aplicación (`App`) local.
        *   Si no, `Net` consulta su `routingTable` para encontrar la entrada correspondiente al destino del paquete.
        *   La tabla provee el `nextNode`. Usando `gateTable`, se determina la interfaz de salida (`toLnk$o[gate]`) hacia ese `nextNode`.
        *   El paquete se reenvía a través de la interfaz seleccionada. Si no se encuentra una ruta al destino, el paquete es descartado (aunque con DV, eventualmente todos los nodos alcanzables deberían tener una ruta).

Esta implementación permite que la red se adapte dinámicamente a la topología y converja hacia las rutas de menor costo (saltos) entre todos los pares de nodos.

## Análisis - segunda parte
En esta sección, evaluamos el rendimiento de la red utilizando el algoritmo de enrutamiento por Vector de Distancias (DV) implementado.

**Número de Saltos:**
Con el enrutamiento DV, se espera una reducción en el número de saltos para las rutas que eran subóptimas en la primera parte (enrutamiento fijo horario). Por ejemplo:
*   **Nodo 0 al Nodo 5:** En la primera parte, la ruta era 0→1→2→3→4→5 (5 saltos). Con DV, la ruta más corta es 0→7→6→5 (3 saltos).
*   **Nodo 2 al Nodo 5:** La ruta 2→3→4→5 (3 saltos) ya era óptima, por lo que no cambia.
En general, para una red en anillo de 8 nodos, el algoritmo DV asegura que el número máximo de saltos entre dos nodos cualesquiera es 4. Esto representa una mejora significativa en la eficiencia de las rutas en comparación con el enrutamiento unidireccional.

**Retraso de Entrega (Delay):**
El gráfico `/images/graph/delay-segunda-parte.png` muestra el retraso de entrega en el Nodo 5 para los dos casos de estudio con enrutamiento DV.

![Delay en Nodo 5 - Segunda parte](/images/graph/delay-segunda-parte.png)

*   **Caso 1 (línea azul):** El retraso se mantiene relativamente bajo y estable, oscilando mayormente por debajo de 25 unidades de tiempo. Las rutas optimizadas y la carga moderada permiten que la red maneje el tráfico hacia el Nodo 5 eficientemente, sin congestión severa. Es una mejora notable respecto a la primera parte, donde la ruta más larga (0→5) contribuiría a un mayor retraso.
*   **Caso 2 (línea naranja):** Se observa una clara tendencia creciente del retraso a lo largo de la simulación, superando las 150 unidades de tiempo. A pesar de que cada paquete toma la ruta más corta, la convergencia de múltiples flujos de tráfico hacia el Nodo 5 (principalmente a través de sus vecinos directos, Nodos 4 y 6) genera una congestión significativa. Esto indica que, si bien el enrutamiento es óptimo, la capacidad de los enlaces de entrada al nodo sumidero se convierte en el cuello de botella.

**Uso del Buffer:**
El enrutamiento por DV redistribuye el tráfico, utilizando enlaces en ambas direcciones del anillo.

*   **Uso del Buffer - Caso 1 (DV):**
    El gráfico `/images/graph/buffer-caso1-segunda-parte.png` muestra la ocupación de los buffers en las interfaces de salida de los Nodos 0 y 2 hacia el Nodo 5.

    ![Buffer en nodos 0 y 2 - Caso 1 - Segunda parte](/images/graph/buffer-caso1-segunda-parte.png)

    *   `Network.node[0].lnk[0]` (línea azul): Representa el buffer del Nodo 0 en la interfaz que lleva al Nodo 7 (ruta antihoraria 0→7→6→5). Muestra actividad considerable con picos de hasta 9-10 paquetes, confirmando el uso de esta ruta más corta. En la primera parte, este enlace estaría inactivo para el tráfico 0→5.
    *   `Network.node[2].lnk[1]` (línea naranja): Corresponde al buffer del Nodo 2 en la interfaz hacia el Nodo 3 (ruta horaria 2→3→4→5). Esta ruta es óptima y no cambia. El buffer muestra una actividad similar.
    Ambos buffers presentan fluctuaciones sin crecimiento descontrolado, lo cual es coherente con el retraso estable observado para el Caso 1.

*   **Uso del Buffer - Caso 2 (DV):**
    El gráfico `/images/graph/buffer-caso2-segunda-parte.png` muestra la ocupación de buffers en varias interfaces clave cuando todos los nodos (excepto el 5) envían tráfico al Nodo 5.

    ![Buffer en nodos - Caso 2 - Segunda parte](/images/graph/buffer-caso2-segunda-parte.png)

    *   Los buffers de los enlaces que alimentan directamente al Nodo 5 son los más críticos: `Network.node[4].lnk[1]` (morado, ruta 4→5) y `Network.node[6].lnk[0]` (marrón, ruta 6→5). Ambos muestran los niveles de ocupación más altos y una clara tendencia creciente, superando los 150-200 paquetes. Esto corrobora que son los principales cuellos de botella, consistente con el retraso creciente del Caso 2.
    *   Enlaces precedentes como `Network.node[3].lnk[1]` (rojo, 3→4) y `Network.node[7].lnk[0]` (rosa, 7→6) también muestran una ocupación alta y creciente.
    *   El enrutamiento DV distribuye la carga hacia el Nodo 5 desde ambas direcciones. Comparado con la primera parte (donde todo el tráfico convergería por una única dirección, por ejemplo, a través del Nodo 4), el DV utiliza mejor la capacidad bidireccional del anillo. Sin embargo, la alta concentración de tráfico en el sumidero sigue provocando congestión.

## Conclusión
La implementación del algoritmo de enrutamiento por Vector de Distancias (DV) en la red en anillo simulada demostró ser efectiva en encontrar y utilizar los caminos más cortos, mejorando la eficiencia general de la red en comparación con el esquema de enrutamiento fijo unidireccional de la primera parte.

1.  **Reducción de Saltos:** El enrutamiento DV logró una disminución significativa en el número de saltos para las rutas que eran subóptimas con el enrutamiento fijo. Esto se traduce en un uso más eficiente de los recursos de la red, ya que los paquetes atraviesan menos nodos intermedios.

2.  **Impacto en el Retraso de Entrega:**
    *   En el **Caso 1**, con carga de tráfico moderada, la combinación de rutas más cortas y la distribución eficiente del tráfico resultó en un retraso de entrega consistentemente bajo y estable. Esto representa una clara mejora sobre el enrutamiento fijo.
    *   En el **Caso 2**, con alta carga de tráfico convergiendo en un único nodo destino (Nodo 5), el retraso de entrega mostró una tendencia creciente a pesar del uso de rutas óptimas. Este resultado subraya que, si bien el enrutamiento DV optimiza las trayectorias, la capacidad de los enlaces de entrada al nodo destino y la capacidad de procesamiento del propio sumidero se convierten en factores limitantes (cuellos de botella) bajo alta demanda.

3.  **Utilización de Buffers:**
    *   El enrutamiento DV permitió una mejor distribución de la carga de tráfico al utilizar enlaces en ambas direcciones del anillo, aprovechando caminos que con el enrutamiento fijo podrían haber estado subutilizados o inactivos para ciertos flujos.
    *   En el **Caso 2**, la congestión se manifestó en los buffers de los enlaces que convergen directamente en el Nodo 5 desde ambas direcciones (ej., `node[4].lnk[1]` y `node[6].lnk[0]`). Aunque la congestión persiste debido a la alta carga concentrada, la distribución del tráfico es preferible a tener una única ruta larga y severamente congestionada, como podría ocurrir con el enrutamiento fijo unidireccional.

En resumen, el algoritmo de Vector de Distancias mejoró la eficiencia de la red en términos de utilización de caminos óptimos y reducción del número de saltos. Los beneficios son más evidentes en escenarios de carga moderada. En escenarios de alta concentración de tráfico hacia un único destino, aunque el enrutamiento es óptimo, la congestión cerca del sumidero sigue siendo un desafío inherente a la carga y la topología de la red, más que una deficiencia del algoritmo de enrutamiento una vez que las rutas óptimas han sido establecidas. Futuros trabajos podrían explorar la implementación de mecanismos de control de congestión para mitigar estos efectos en escenarios de alta demanda.
