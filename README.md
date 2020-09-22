![Banner](https://raw.githubusercontent.com/ViliOrg/Assets/master/banners/vili_banner_small.png)

# Vili
A simplistic readable data format !

![build](https://github.com/Sygmei/Vili/workflows/build/badge.svg)

## Description

Vili is a YAML-like data language without all the complexity, it comes with handy features allowing you to describe your data with beautiful tree-structures.

# Examples :

## Animals (Simple tree-like structure)

### Vili file

`Animalia.vili`

```yaml
Animalia:
    Chordate:
        Mammal:
            Carnivora:
                Felidae:
                    Felis:
                        Domestica:
                            HouseCat:
                                name: "House Cat"
                                size: 24.5
                                cool: true
                        Leo:
                            Lion:
                                name: "Lion"
                                size: 280.7
                                cool: true
        Primate:
            Pongidae:
                Pan:
                    Troglodytes:
                        Chimpanzee:
                            name: "Chimpanzee"
                            size: 81.6
                            cool: true
            Hominidae:
                Homo:
                    Sapiens:
                        Human:
                            name: "Human"
                            size: 170
                            cool: false
    Arthopoda:
        Insect:
            Diptera:
                Muscidae:
                    Musca:
                        Domestica:
                            Housefly:
                                name: "Housefly"
                                size: 0.12
                                cool: false
```

## Countries and Cities (Lists)

### Vili file

`Countries.vili`

```yaml
# Inline
France:
    cities: ["Paris", "Marseille", "Lyon", "Nice"] 
# Multiline
Germany:
    cities: [
        "Berlin",
        "Hamburg",
        "Munich",
        "Cologne"
    ]
# Do however you want
UnitedKingdom:
    cities: [
        "London", "Manchester",
        "Liverpool", "Glasgow"
    ]
```


## Templates

```yaml
# Simple templates (aliases)
template on: true
template off: false

switch_1: on
switch_2: off

# Advanced templates
template color: {
    type: "color",
    r: 0,
    g: 0,
    b: 0,
    a: 255
}

black: color
red: color { r: 255 }
green: color { g: 255 }
blue: color { b: 255 }
yellow: color { r: 255, b: 255 }
```