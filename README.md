# Projekt na zaliczenie poniższych przedmiotów:
* **Techniki mikroprocesorowe**
* **Techniki sensoryczne**
* **Systemy kontrolno pomiarowe**


## Sposób wysyłania komend:
Komenda wysłana do uP nie może przekraczać 16 znaków ASCI!
### Ramka:
```
   [   1 znak     ] [ 1 znak ] [ max 13 znaków ] [ 1 znak ]
   [Device address] [ command] [      args     ] [  '\n'  ]
```

* [Device address]:
    * E - silniki
    * U - ultra sonic
    * A - akcelerator

### Komendy dla silników:
```
E [command] [arg]
```
* l [odległość jaką ma przejechać] - silnik lewy 
* r [odległość jaką ma przejechać] - silnik prawy 
* f [odległość jaką ma przejechać] - oba silniki
* a [kąt o jaki ma się obrócić]    - obrót pojazdu w osi

### Komendy dla Sensora odległości:
```
U [command] [arg]
```
* r [kąt] - obróć o zadany kąt
* m [] - dokonaj pomiaru

### Komendy dla akcelerometru:
```
A [command] [arg]
```
* x [] - odczytaj oś x
* y [] - odczytaj oś y
* z [] - odczytaj oś z
* w [address data] - zapisz do rejestru o adresie [address] wartość [data]
* r [address] - odczytaj z rejestru o adresie [address]
    
