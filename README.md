# ViliData
A nice and readable data format !

# Examples :

## A color model :

```
$Color:
    @ValueRef
        __linkroot__:"/__init__"
        
    @ColorComposant
        type:"Int"
        value:0
        @typeConstraints
            minValue:0
            maxMalue:255

    @DefaultOptional
        optional:True
        defaultValue: 255

    @__init__
        @0:ColorComposant
        @1:ColorComposant
        @2:ColorComposant
        @3:ColorComposant,DefaultOptional

    @__body__:ValueRef
        r:&(0/value), g:&(1/value), b:&(2/value), a:&(3/value)
```

## A simple animation file :

```
Flag (Anim);
Flag (Lock);

Meta:
    name:"CLIC", clock:10, play-mode:"FORCE"
Images:
    indexes:[1..9], model:"<x>.png"
Groups:
    @main
        content:[8..0]
Animation:
    code:[
        "PLAY_GROUP(main, 1)",
        "CALL('HOLD')"
    ]
```
