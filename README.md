# MrHAT-PIC18Q20


## Power IC system RESET

```
ON PIC:
if QON button pressed with 1S 1S 1L pattern
    - system power reset requested from power IC
```

## Go to ship mode

### 1->2->3
ON PIC:
```
1.)if QON button pressed with 1L pattern
    - reset MCU_INT pin
    - write SHUTDOWN REQ to internal register

3.)if PI released PI_RUN and SHUTDOWN ACK == 1
        - put the power IC to ship mode, it will wait 10sec before go to ship mode


```

ON PI
```

Toggle PI_RUN in every 300msec

2.)in case of MCU_INT falling edge
    - if SHUTDOWN REQ active 
        - set SHUTDOWN ACK register
        - call system shutdown
            - after it the PI will release all pins

```