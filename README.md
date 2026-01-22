# proyecto-progra — Sistema de Ventas (C)

Sistema de ventas por consola desarrollado en **C** para un “puesto de ventas”.  
Permite gestionar **productos**, **ventas (facturas)**, **inventario**, **reportes** y **caja**, con guardado/carga automática de datos en archivos **CSV**.

---

## ✨ *Funcionalidades*

### 1) Módulo de Productos
- Registrar productos (nombre, precio, stock, IVA 0 / 0.12 / 0.15).
- Listar productos en formato de tabla.
- Actualizar productos:
  - Cambiar nombre, precio, IVA.
  - Activar / desactivar producto (activo = 1, inactivo = 0).

### 2) Módulo de Ventas (Facturas)
- Crear una **factura** con **varios productos**.
- Búsqueda de productos para vender por:
  - índice, ID o nombre exacto.
- Control de stock (no permite vender si no hay stock suficiente).
- Calcula por ítem:
  - subtotal, IVA, total.
- Imprime factura con:
  - totales generales y totales por tipo de IVA.
- Consultar una factura por ID.

### 3) Módulo de Inventario
- Ajuste de stock (positivo o negativo) por:
  - índice, ID o nombre exacto.
- Evita dejar stock en negativo.

### 4) Módulo de Reportes
- Totales de ventas por tipo de IVA (0%, 12%, 15%).
- Reporte de productos con stock bajo (según límite que se ingresa).
- Historial de items vendidos (detalle de cada venta con fecha/hora).

### 5) Módulo de Caja
- Abrir caja con monto inicial.
- Cerrar caja mostrando:
  - total vendido
  - total final (caja inicial + vendido)
- No permite vender si la caja está cerrada.

---

## 🧩 Archivos del proyecto

- `ventas_epnt_v2.c` → código fuente principal.
- `productos.csv` → se genera/actualiza automáticamente al salir.
- `ventas.csv` → se genera/actualiza automáticamente al salir.

> Si `productos.csv` y `ventas.csv` existen, el programa los carga automáticamente al iniciar.

---
## para usar mis programa

## imagenes 

## ▶️ Compilación y ejecución (Windows / Linux / macOS)

### Opción con GCC (recomendado)

**Compilar:**
```bash
gcc ventas_epnt_v2.c -o ventas

