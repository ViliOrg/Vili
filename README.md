# ViliData
A nice and readable data format !

# Examples :

## Animals (Simple tree-like structure)
```
Animalia:
  Chordate:
    Mammal:
      Carnivora:
        Felidae:
          Felis:
            Domestica:
              HouseCat:
                name:"House Cat"
                size:24.5
                cool:True
            Leo:
              Lion:
                name:"Lion"
                size:280.7
                cool:True
    Primate:
      Pongidae:
        Pan:
          Troglodytes:
            Chimpanzee:
              name:"Chimpanzee"
              size:81.6
              cool:True
      Hominidae:
        Homo:
          Sapiens:
            Human:
              name:"Human"
              size:170
              cool:False
  Arthropoda:
    Insect:
      Diptera:
        Muscidae
          Musca:
            Domestica:
              Housefly:
                name:"Housefly"
                size:0.12
                cool:False
```
## Countries and Cities (Lists)

```
France:
  cities:["Paris", "Marseille", "Lyon", "Nice"]
Germany:
  cities:["Berlin", "Hamburg", "Munich", "Cologne"]
UnitedKingdom:
  cities:["London", "Manchester", "Liverpool", "Glasgow"]
```

## Little company (Inheritance)

```
Employees:
  TeamBusiness:
    team:"Business"
    rank:1
  TeamDeveloppers:
    team:"Developpers"
    rank:2
  TeamSupport:
    team:"Support"
    rank:3
  Bob(TeamBusiness):
    name:"Bob Dupont"
  Mark(TeamDeveloppers):
    name:"Mark Mark"
  Gregory(TeamSupport):
    name:"Gregory No"
```

## Colors (Templates)

`ColorTemplate.vili`

```
Color:    
    IntComposant:
      type:"Int"
    FaculativeIntComposant(IntComposant):
      defaultValue:255

    __init__:
        0(IntComposant):
        1(IntComposant):
        2(IntComposant):
        3(FaculativeIntComposant):

    __body__:
        type:"Color"
        r:&(0/value)
        g:&(1/value)
        b:&(2/value)
        a:&(3/value)

Template(Color);
```

`Color.vili`

```
Include (ColorTemplate);

black:Color(0, 0, 0)
white:Color(255, 255, 255)
red:Color(255, 0, 0)
green:Color(0, 255, 0)
blue:Color(0, 0, 255)
yellow:Color(255, 255, 0)
cyan:Color(0, 255, 255)
magenta:Color(255, 0, 255)
transparent:Color(0, 0, 0, 0)
transparent_red:Color(255, 0, 0, 128)
```

## File list (Ranges)

```
Images:
  files:[0..499]
  model:"%s.png"
Documents:
  files:[99..63]
  model:"%s.doc"
```
