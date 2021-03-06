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
### Matériel
Voici les composants nécessaires à la construction du tasse-o-tron
| Composant         | Rôle          |
| :----:            |    :---    | 
| ESP32            | MicroContrôleur         |
| Jauge à LED         | Indicateur de la température actuelle de la boisson, sur base de pourcentage | 
| DHT11       | Capteur de température, qui détecte celle de la boisson posée | 
| Ecran LCD      | Ecran qui affiche des messages d'informations, notamment lors de l'initialisation, puis la température idéale sélectionnée | 
| Potentiomètre | Tourner le potentiomètre permet de modifier la température à laquelle vous souhaitez être notifié |
| Photorésistance | Détecte la luminosité, qui servira à déterminer si une tasse est posée ou non sur le support |
### Librairies
Certaines librairies sont nécessaires pour assurer le bon fonctionnement du produit
| Librairie         | Embarquée dans ArduinoIDE          |
| :---            |    :---    | 
| Arduino            | Oui         |
| DHT            | Oui         |
| LiquidCrystal            | Non [(liquid_crystal.zip)](./liquid_crystal.zip)        |
| WiFi            | Oui         |
| WiFiMulti            | Oui         |
### Branchements
![Cablage des composants](./cablage.jpg)

Les pins des éléments sont modifiables, il faudra simplement penser à changer les constantes associées dans le code.
> :warning: Pour les pins en INPUT, il est nécessaire de faire le branchement sur un pin **ADC1**, les pins ADC2 en input et le module wifi ne peuvent pas fonctionner simultanément

![ESP32 pins](./esp32.png)
### Code
#### Constantes
Comme mentionné ci-haut, pensez à mettre le bon numéros de pin dans les constantes associées. Concernant la jauge à led, il s'agit d'un tableau d'entiers : la première valeur correspond au pin de la led la plus basse, la dernière à la led la plus haute.

#### Identifiants WIFI
Le tasse-o-tron utilise une connexion wifi pour envoyer sa notification, il faut donc lui fournir l'identifiant et le mot de passe pour s'y connecter

![Code wifi](./wifi.png)

### Notification
Pour envoyer la notification indiquant que la boisson est à température optimale, le site [ifttt](https://ifttt.com/create) est utilisé. Créez votre propre évènement en utilisant un webhook pour le **if then**, et un système de messagerie de votre choix pour le **then that**. Copiez-collez l'url du webhook dans la constante MSG_URL

![Url IFTTT](./msg_const.png)