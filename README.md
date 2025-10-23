# Examen Práctico: Control de Sala Avanzado

**Curso:** 4100901 - Estructuras Computacionales

---

## ✅ Parte 1: Actividades Técnicas a Realizar (80 puntos)

### 1. Mensaje de bienvenida personalizado \[10 puntos\]

Modifique `room_control_app_init()` para imprimir al inicio del sistema:

```
Controlador de Sala v2.0
Estado inicial:
 - Lámpara: 20%
 - Puerta: Cerrada
```

El duty cycle inicial debe ser 20% en lugar del actual 0%. Asegúrese de que el mensaje se envíe después de la inicialización de UART.

**Recomendaciones:**
- Actualice `PWM_INITIAL_DUTY` en `room_control.h`
- Use `uart_send_string()` para enviar el mensaje completo

---

### 2. Comando UART para estado del sistema \[10 puntos\]

Implemente el comando `'s'` para que por UART se imprima el estado actual:

```
Estado actual:
 - Lámpara: XX%
 - Puerta: Abierta/Cerrada
```

El porcentaje debe reflejar el duty cycle real del PWM, y el estado de puerta debe basarse en el estado de ocupación (Abierta si OCCUPIED, Cerrada si IDLE).

**Recomendaciones:**
- Agregue el caso en `room_control_on_uart_receive()`
- Use variables globales para acceder al duty cycle actual
- Implemente una función auxiliar para obtener el duty cycle actual

---

### 3. Comando de ayuda UART \[10 puntos\]

Implemente el comando `'?'` para mostrar un menú de ayuda:

```
Comandos disponibles:
 '1'-'5': Ajustar brillo lámpara (10%, 20%, 30%, 40%, 50%)
 '0'   : Apagar lámpara
 'o'   : Abrir puerta (ocupar sala)
 'c'   : Cerrar puerta (vaciar sala)
 's'   : Estado del sistema
 '?'   : Ayuda
```

**Recomendaciones:**
- Incluya todos los comandos existentes más los nuevos
- Use múltiples llamadas a `uart_send_string()` para el mensaje largo

---

### 4. Control automático de iluminación con restauración \[10 puntos\]

Al presionar el botón B1, la lámpara debe encenderse al 100%. Después de 10 segundos, debe volver al brillo anterior configurado por UART (no necesariamente 0%).

**Recomendaciones:**
- Guarde el valor anterior del PWM antes de cambiar a 100%
- Use `systick_get_ms()` para el temporizador de 10 segundos
- Modifique `room_control_update()` para manejar este timeout específico

---

### 5. Transición gradual de brillo \[10 puntos\]

Implemente el comando `'g'` para que la lámpara aumente gradualmente de 0% a 100% en pasos de 10% cada 500ms.

**Recomendaciones:**
- Use un estado adicional en la máquina de estados o flags para manejar la transición
- Implemente temporizadores en `room_control_update()`
- El comando debe funcionar independientemente del estado de ocupación

---

### 6. Bit de paridad UART \[15 puntos\]

Modifique la configuración de UART para activar bit de paridad impar (odd parity), mejorando la confiabilidad en ambientes ruidosos.

**Recomendaciones:**
- Consulte la documentación del registro `CR1` de USART2
- Verifique que la comunicación siga funcionando correctamente

---

### 7. Remapeo del pin PWM \[15 puntos\]

Cambie la configuración del PWM de PA6 (TIM3_CH1) a PB4 (que también soporta TIM3_CH1) como nuevo pin de salida PWM.

**Recomendaciones:**
- Actualice `gpio.h`, `gpio.c`, y `tim.c`
- Verifique el Alternate Function de PB4 en el manual de referencia
- Asegúrese de que el PWM siga funcionando en el nuevo pin


---
