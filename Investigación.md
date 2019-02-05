# Investigación Proyecto Driving Coach

## Futuras características
* Control de la ruta que sigue. Poder responder cuantó le queda para llegar al destino, avisar en caso de sobrepasar la velocidad o de haber posibles atascos con la API de Google Maps.
* Detección conductor distraido (texteando, hablando por el teléfono, operar en la radio, beber, hablar con el pasajero...)
* Depende de la situación, lanzar un aviso. Si se está durmiendo, si fuese un smart-car se podría ordenar a parar. Si no, se puede elaborar un diálogo en el que se le preguntan cosas para que no se duerma. O se le puede ofrecer sugerencias de donde parar en la ruta y cuánto tiempo perdería.
* Elaborar un chatbot para poder realizar acciones como cambiar la música del móvil, poner recordatorio... Simplemente añadiendo la funcionalidad de Google Assistant.

## Plataforma
Se debería ver si sería factible hacerlo con una web-app, si va muy lento a través de un servidor de Google.

En caso de no ser factible por baja calidad de la cámara, por ejemplo, se podría usar una raspberry pi con una video cámara.

## Fuente de datos disponible (desde un lateral)
- [Kaggle](https://www.kaggle.com/c/state-farm-distracted-driver-detection/data)

Consta de 102.000 imágenes categorizadas en 10 clases:
* c0: safe driving
* c1: texting - right
* c2: talking on the phone - right
* c3: texting - left
* c4: talking on the phone - left
* c5: operating the radio
* c6: drinking
* c7: reaching behind
* c8: hair and makeup
* c9: talking to passenger

## Convolutional Neural Networks:
* [Explicación](https://ujjwalkarn.me/2016/08/11/intuitive-explanation-convnets/)
* [Inception Network](https://towardsdatascience.com/a-simple-guide-to-the-versions-of-the-inception-network-7fc52b863202)
* [La mejor hasta el momento - Inception v4](https://towardsdatascience.com/review-inception-v4-evolved-from-googlenet-merged-with-resnet-idea-image-classification-5e8c339d18bc)

## Tecnologías a usar
* [Keras](https://keras.io/)
* [TensorFlow](https://www.tensorflow.org)

## Proyecto similar, pero sólo en función de detección de distracciones:
* [1º parte](https://software.intel.com/en-us/articles/ai-developer-project-part-1-combating-distracted-driver-behavior): Se explica cual va a ser el plan de desarrollo y cuales son los objetivos del dispositivo en el que se implementará el software.
* [2º parte](https://software.intel.com/en-us/articles/ai-developer-project-part-2-combating-distracted-driver-behavior): Se explica de donde se van a coger los datos y que método de detección usarán, Inception…
* [3º parte](https://software.intel.com/en-us/articles/ai-developer-project-part-3-combating-distracted-driver-behavior) y [4º parte](https://software.intel.com/en-us/articles/ai-developer-project-part-4-combating-distracted-driver-behavior): Ajuste de parámetros y mejora

## Producto real más parecido
* [Soteria](http://soteriasafedriving.com/)
