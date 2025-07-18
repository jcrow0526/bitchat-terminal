# BitChat for Flipper Zero

Una adaptación de BitChat para funcionar en Flipper Zero, permitiendo chat descentralizado a través de Bluetooth LE.

## Características

- **Chat Mesh Descentralizado**: Los mensajes se retransmiten a través de peers para llegar a todos
- **Cifrado de Extremo a Extremo**: Mensajes privados cifrados con X25519 + AES-256-GCM  
- **Sin Internet**: Funciona completamente offline a través de Bluetooth LE
- **Canales**: Chats grupales públicos y protegidos por contraseña
- **Privacidad**: Sin cuentas, sin rastreo, sin números de teléfono
- **Compatible**: Funciona con las apps de iOS/Android de BitChat

## Instalación

1. Compila el firmware personalizado o usa qFlipper
2. Copia el archivo `.fap` a la carpeta `apps/Bluetooth/` de tu Flipper
3. Ejecuta la app desde el menú de Bluetooth

## Uso

### Navegación
- **OK**: Seleccionar/Confirmar
- **Back**: Volver/Cancelar  
- **Up/Down**: Navegar menús
- **Left/Right**: Cambiar entre conversaciones

### Funciones Principales
- **Chat**: Ver y enviar mensajes
- **Peers**: Lista de dispositivos conectados
- **Settings**: Configurar nickname y opciones

### Limitaciones del Flipper

Debido a las limitaciones de hardware del Flipper Zero:

- **Memoria limitada**: Máximo 50 mensajes en historial
- **Sin teclado completo**: Entrada de texto simplificada
- **Pantalla pequeña**: Interfaz optimizada para 128x64
- **CPU limitada**: Sin compresión ni funciones avanzadas
- **Bluetooth básico**: Implementación BLE simplificada

## Estructura del Proyecto

```
src/
├── flipper_main.c          # Punto de entrada principal
├── bitchat_app.h/c         # Estructura principal de la app
├── bitchat_core.h/c        # Lógica central de BitChat
├── scenes/                 # Escenas de la interfaz
│   ├── bitchat_scene.h/c   # Manejador de escenas
│   ├── bitchat_scene_*.c   # Escenas individuales
│   └── bitchat_scene_config.h
└── views/                  # Vistas personalizadas
    └── bitchat_view.h/c
```

## Desarrollo

Para compilar:

```bash
# En el directorio del firmware de Flipper
./fbt fap_bitchat
```

Para instalar:

```bash
./fbt launch_app APPSRC=applications_user/bitchat
```

## Compatibilidad

Esta versión es compatible con:
- Flipper Zero con firmware oficial
- Apps de BitChat para iOS/Android
- Versión terminal de BitChat (Rust)

## Limitaciones Conocidas

- **Entrada de texto**: Limitada por la interfaz del Flipper
- **Historial**: Solo 50 mensajes por limitaciones de memoria
- **Criptografía**: Implementación simplificada
- **Red mesh**: Funcionalidad básica de retransmisión

## Contribuir

Las PRs son bienvenidas. Asegúrate de mantener compatibilidad con iOS/Android.

## Licencia

Dominio Público

---

Basado en el proyecto original BitChat por [@jackjackbits](https://github.com/jackjackbits)