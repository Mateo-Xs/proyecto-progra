# 🧾 Poli Steel — Sistema de Ventas e Inventario (C)
**autores: Mateo Guamanzara, Adrian Clonares, Andre Rosero**
## 👥 Autores

- **Andre Rosero** — GitHub: [@andrerosero22z-pixel](https://github.com/andrerosero22z-pixel)
- **Mateo Guamanzara** — GitHub: [@Mateo-Xs](https://github.com/Mateo-Xs)
- **Adrian Clonares** — GitHub: [@adriaan-07](https://github.com/adriaan-07)


- *INFORMACION AUTORES*: Estudiantes de la Escuela Politecnica nacional, de la carrera de ingeneria en software y ingeneria en ciencia de datos e inteligencia artificial.

Sistema de **punto de venta por consola** hecho en **C** para un “puesto de ventas”.  
La idea es simple: tener un catálogo de productos, manejar inventario, abrir/cerrar caja y registrar ventas, todo desde menús.

Este repo corresponde al proyecto del **primer bimestre** y en el **segundo bimestre** se añadió lo que el profe pidió: **búsqueda + ordenamiento**, y  **CSV**.

![C](https://img.shields.io/badge/Language-C-00599C?logo=c&logoColor=white)
![License](https://img.shields.io/badge/License-Check%20LICENSE-lightgrey)

[![License](https://img.shields.io/github/license/andrerosero22z-pixel/NOMBRE_DEL_REPO)](LICENSE)


---
[![Compilación](https://img.shields.io/badge/Ir%20a-Compilaci%C3%B3n-blue)](#%EF%B8%8F-compilación-y-ejecución)
[![Uso](https://img.shields.io/badge/Ir%20a-Uso%20r%C3%A1pido-green)](#FUNIONALIDADES)
[![Checklist](https://img.shields.io/badge/Ir%20a-Checklist-orange)](#-checklist-lo-que-pidió-el-profe)




## ✨ FUNCIONALIDADES

### 🧩 Productos
- Registrar productos (código, nombre, precio, stock, stock mínimo, tipo de IVA).
- Listar productos en tabla.
- Editar/actualizar datos del producto (según opciones del menú).

### 📦 Inventario
- Aumentar stock.
- Disminuir stock con validación (no dejar stock negativo).

### 🧾 Ventas
- Solo permite vender si la **caja está abierta**.
- Venta con **varios productos** por factura.
- Valida: producto existente, cantidad válida y stock disponible.
- Calcula subtotal, IVA (si aplica) y total.

### 💵 Caja
- Abrir caja con monto inicial.
- Ver estado.
- Cambiar el IVA general (por defecto 12%).
- Cerrar caja y mostrar totales.

### 📊 Reportes
- Totales vendidos por tipo de IVA.
- Productos con stock bajo (según stock mínimo).

---

## 🔎 Algoritmos (requisito del examen)

### Búsqueda
- **Búsqueda binaria**  (para encontrar productos y validar registros).

### Ordenamiento
- **Selection Sort** (Centro de Ordenamiento)
  - Ordena el catálogo (ej. por **precio** o **stock**) según la opción del menú.

---

## 🧩 Estructura del proyecto

- `principal.c` → archivo principal del sistema.


---

## ▶️ Compilación y ejecución

### ✅ Windows (GCC / MinGW / MSYS2)
```bash
gcc principal.c -o poli_steel -std=c11
./poli_steel


