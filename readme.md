# Welcome to the TASSE-O-TRON
```
++++++++++++++++++++++++++++++++++++++++++++
+++++                                  +++++
++++    ####  ###  ##### ##### #####    ++++
+++      ##  ## ## ##    ##    ##        +++
++       ##  ##### ##### ##### ###        ++
+        ##  #   #    ##    ## ##          +
+        ##  #   # ##### ##### #####       +
+                                          +
+                 #####                    +
+                 ## ##                    +
+        -O0O-    ## ##      -O0O-         +
+                 ## ##                    +
+                 #####                    +
+                                          +
+         #### ##### ##### ##  #           +
+          ##  ##  # ## ## ##  #           +
++         ##  ####  ## ## # # #          ++
+++        ##  ##  # ## ## #  ##         +++
++++       ##  ##  # ##### #  ##        ++++
+++++                                  +++++
++++++++++++++++++++++++++++++++++++++++++++
```
## Cup of coffee ? 'Kay, but DRINK IT !
Connaissez-vous cette situation où vous vous faites chauffer un café ou un thé, une tisane ou un chocolat chaud, et vous posez votre tasse à côté de vous en attendant qu'elle refroidisse un peu. Les minutes passent, d'abord par cinq, puis par dix, puis par vingt... Et finalement, votre savoureuse boisson ne pourrait plus réchauffer ne serait-ce qu'un glaçon.

Si vous partagez cette douleur des tasses froides, séchez vos larmes et venez découvrir le tasse-o-tron, l'ami qui veillera sur votre précieux élixir !

## Je serai vos yeux et votre thermomètre ʕ•ᴥ•ʔ
Le tasse-o-tron est un support de tasse qui détecte en continu sa température, et vous avertit dès que la température que vous jugez optimale est atteinte. Car oui, le tasse-o-tron ne prétend pas connaître vos goûts, il vous laisse choisir ce qui est bon pour vous ! Le processus est simple :
1. Allumez le tasse-o-tron
2. Réglez la température à laquelle vous aimez boire votre café/thé/tisane/chocolat
3. Posez votre tasse
4. Recevez une notification au meilleur timing possible
4. Dégustez votre boisson
4. Éteignez le tasse-o-tron s'il n'est plus utilisé, c'est pas Versaille ici ʕ •́؈•̀ ₎ 

## Build me please (∩｀-´)⊃━☆ﾟ.*･｡ﾟ
Voici les composants nécessaires à la construction du tasse-o-tron
| Composant         | Rôle          |
| :----:            |    :---    | 
| ESP32            | MicroContrôleur         |
| Jauge à LED         | Indicateur de la température actuelle de la boisson, sur base de pourcentage | 
| DHT11       | Capteur de température, qui détecte celle de la boisson posée | 
| Ecran LCD      | Ecran qui affiche des messages d'informations, notamment lors de l'initialisation, puis la température idéale sélectionnée | 
| Potentiomètre | Tourner le potentiomètre permet de modifier la température à laquelle vous souhaitez être notifié |
| Photorésistance | Détecte la luminosité, qui servira à déterminer si une tasse est posée ou non sur le support |