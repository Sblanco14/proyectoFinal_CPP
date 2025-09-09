# INFORME FINAL – Sistema de Monitoreo Biomédico

> **Autores:** Santiago Blanco – Paula Sofía Niño  
> **Curso:** Programación avanzada  
> **Repositorio:** `ProyectoFinal_C++` (carpeta raíz del código: `proyecto/`)

---

## 1. Introducción y Objetivos
Este proyecto implementa, en C++ y sin STL avanzada, un sistema que:
1) Lee **umbrales** de sensores desde `configuracion.txt` y **pacientes** desde `pacientes.csv`.
2) Carga lecturas biomédicas desde archivos binarios **`.bsf`** (UCI: sala → máquinas → mediciones → lecturas).
3) **Detecta anomalías** para T/P/O usando umbrales y determina **ECG anómalo** con la regla definida en el enunciado.
4) Genera **reportes de texto** y **exporta** pacientes con ECG anómalo a un binario (`pacientes_ecg_anomalos.dat`).

**Enfoque didáctico:** uso de **structs**, **arreglos dinámicos**, **punteros**, **C-strings**, y **fstream** (texto/binario).  
`std::string` se emplea solo para **tokenizar/parsear** líneas de archivos de texto.

---

## 2. Diseño de Datos (Structs principales)
A continuación se detallan las estructuras clave; todas residen en `proyecto/libs/`.

### 2.1 Tamaños fijos (`tamanos.h`)
```cpp
const int ID_PACIENTE_LEN = 12;     // 11 + '\0'
const int FECHA_HORA_LEN  = 25;     // 24 + '\0'
const int RUTA_LEN        = 260;    // rutas de archivos
```

### 2.2 Lecturas y mediciones
- **`lectura.h`**
```cpp
struct Lectura { char tipoSensor; double valor; int32_t sistolica; int32_t diastolica; };
```
- **`medicion.h`**
```cpp
struct Medicion {
    char idPaciente[ID_PACIENTE_LEN];
    char fechaHora[FECHA_HORA_LEN];
    uint32_t numLecturas;
    Lectura* lecturas; // dinámico
};
```

### 2.3 Máquina y sala UCI
- **`maquina.h`**
```cpp
struct MaquinaUCI { uint8_t id; uint32_t numMediciones; Medicion* mediciones; };
```
- **`sala.h`**
```cpp
struct Sala { uint8_t idSala; uint8_t numMaquinas; MaquinaUCI* maquinas; };
```

### 2.4 Umbrales y configuración
- **`umbral.h`**
```cpp
struct Umbral { char tipoSensor[16]; double min; double max; };
```
- **`config.h`** (estructura de conveniencia para rutas y límites; no estrictamente requerida por el flujo principal):
```cpp
struct Config {
    char rutaDatosBSF[RUTA_LEN];
    char rutaPacientesTxt[RUTA_LEN];
    char rutaReporteTxt[RUTA_LEN];
    double tempMinValida, tempMaxValida, ecgMinValido, ecgMaxValido;
    int32_t oxiMinValido, oxiMaxValido;
    uint8_t activarLogsBinarios, activarReporteDetallado;
};
```

### 2.5 Pacientes
- **`paciente.h`** y **`archivo_pacientes.h`**
```cpp
struct Paciente { /* id, tipoDocumento, documento, nombres, apellidos, ... */ };
struct ArchivoPacientes { int numPacientes; Paciente* pacientes; };
```

---

## 3. Funciones Implementadas (por módulo)

### 3.1 Carga de configuración y pacientes
- **`funcionesConfig.h`**
  - `parseLineaConfiguracion(const char*) → Umbral`
    - Tokeniza una línea `TIPO,MIN,MAX` (ignora `#`/líneas vacías).
    - Copia `TIPO` a `char tipoSensor[16]` y transforma MIN/MAX a `double`.
- **`funcionesPacientes.h`**
  - `parseLineaPaciente(const char*) → Paciente`
    - Divide por `;` en 11 campos, `trim`, y copia a `char[]`.
    - Evita overflows con `strncpy` y terminador `\0`.
- **`cargarDatos.h`**
  - `cargarUmbrales(const char*, Umbral*& ) → int`
    - Lee archivo de texto y construye **arreglo dinámico** de `Umbral` con estrategia de **doble capacidad**.
  - `cargarPacientes(const char*) → ArchivoPacientes`
    - Ídem para `Paciente`. Devuelve `ArchivoPacientes` con `pacientes` dinámico.

### 3.2 Fechas y ordenamiento
- **`funcionesFecha.h`**
  - `compararFechas(f1,f2) → bool`  
    Retorna `true` si `f1 > f2` (compara `AAAA/MM/DD HH:MM:ss.sss`).
- **`ordenar.h`**
  - `ordenarMedicionesPorFecha(Medicion*, int)`  
    Inserción: desplaza mientras `arr[j] > key` usando `compararFechas`.

### 3.3 Anomalías (no ECG)
- **`anomalias.h`**
  - `esAnomalaNoECG(const Lectura&, const Umbral*, int) → bool`  
    - `T`: compara `valor` contra `[min,max]` del umbral `"T"`.
    - `O`: compara `valor` contra `"O"`.
    - `P`: compara `sistolica` con `"P_SIS"` y `diastolica` con `"P_DIA"`.

### 3.4 Anomalía ECG y exportación
- **`anomalias_ecg.h`**
  - `minMaxECGEnMaquina(const MaquinaUCI&, const char*, double& min, double& max)`
  - `ecgAnomaloPacienteEnMaquina(const MaquinaUCI&, const char*, const Umbral*, int) → bool`  
    **Regla ECG:** `|minECG| + |maxECG| > |umbral.min| + |umbral.max|` ⇒ anómalo.
- **`export_ecg.h`**
  - `contarECG(const MaquinaUCI&, const char*) → uint32_t`
  - `yaVistoPacienteEnAnterior(const MaquinaUCI&, uint32_t, const char*) → bool` (evita duplicados)
  - `exportarPacientesECGAnomalos(const Sala&, const Umbral*, int, const char*="pacientes_ecg_anomalos.dat") → bool`
    - Formato del BIN por bloque: `ID (11B)` + `N (4B)` + repeticiones de (`fecha 24B` + `double 8B`).

### 3.5 Reportes
- **`reporteAnomalias.h`**
  - `generarReporteAnomalias(const Sala&, const Umbral*, int, const char*="anomalias.txt") → bool`
    - Recorre todas las máquinas/mediciones/lecturas; incluye **solo T/P/O anómalas**.
- **`reportes.h`**
  - `generarReporteMedicionesPaciente(id, mediciones[], n, umbrales[], nU, outPath)`
    - Ordena por fecha, agrupa por sensor, y al final de cada grupo calcula **min/max/prom**.

### 3.6 Lectura/gestión de BSF y memoria
- **`leerBSF.h`**
  - `leerArchivoBSF(const char*, Sala&) → bool`  
    Carga `Sala → MaquinaUCI* → Medicion* → Lectura*` desde el `.bsf`.
    - Validación: aborta si `numLecturas` es inválido (p. ej., `> 10000`).
  - `liberarSala(Sala&)`  
    Libera **toda** la memoria dinámica anidada.

### 3.7 Validador (herramienta auxiliar)
- **`validate_export_file (1).cpp`**
  - `ValidateEcgExportFile(const char* bin, const char* txt) → bool`  
    Lee `pacientes_ecg_anomalos.dat` y produce `validation_ecg_anomalies.txt` legible.
  - `main()` genera el TXT con formato claro: ID de paciente, número de anomalías y la lista `(fecha, valor)`.

---

## 4. Flujo de Ejecución (menú principal)
1. **Cargar Config + Pacientes**  
   `cargarUmbrales("data/configuracion.txt")` y `cargarPacientes("data/pacientes.csv")`.
2. **Leer BSF**  
   `leerArchivoBSF("data/patient_readings_simulation 1.bsf")`.
3. **Generar anomalias (T/P/O)**  
   `generarReporteAnomalias(...)` → `anomalias.txt` (agrupado por paciente, ordenado por fecha).
4. **Exportar ECG anómalo (BIN)**  
   `exportarPacientesECGAnomalos(...)` → `pacientes_ecg_anomalos.dat`.
5. **(Opcional) Validar BIN**  
   Ejecutar `./validacion` para crear `validation_ecg_anomalies.txt`.

**Notas:**
- Se asume que los archivos de entrada están en `proyecto/data/`.  
- Orden recomendado: 1 → 2 → 3/4.  
- Cambiar el `.bsf` es tan simple como modificar la ruta en el paso 2.

---

## 5. Detección de Anomalías (detalles)
- **Temperatura (T)** y **Oximetría (O)**: una lectura es **anómala** si `valor < min` o `valor > max` según el umbral del tipo.
- **Presión (P)**: **sistólica** se compara con `P_SIS` y **diastólica** con `P_DIA`; es anómala si alguna excede su umbral.
- **ECG (E)**: no se marca anómalo lectura por lectura; se evalúa por paciente/ máquina:  
  Si `|minECG| + |maxECG| > |umbral.min| + |umbral.max|` ⇒ **ECG anómalo**.

---

## 6. Estadísticas
En reportes por paciente se calculan **mínimo, máximo y promedio** para cada sensor (excepto ECG donde se listan valores):
- `estadisticas.h`: `StatsD`/`StatsI` con `init`, `acumular`, `promedio`.

---

## 7. Gestión de Memoria
- Todas las colecciones (`Medicion*`, `Lectura*`, `MaquinaUCI*`, `Paciente*`, `Umbral*`) son **dinámicas**.
- Se evita STL y se aplica patrón de **duplicación de capacidad** en carga de texto (umbrales/pacientes).
- Limpieza:
  - `delete[] umbrales;`
  - `delete[] AP.pacientes;`
  - `liberarSala(sala);`  
  que libera `lecturas[]`, luego `mediciones[]`, luego `maquinas[]`.

---

## 8. Archivos de Entrada y Salida
**Entrada** (`proyecto/data/`):
- `configuracion.txt` → umbrales (`T,36.0,38.0`, `P_SIS,90,140`, ...)
- `pacientes.csv` / `pacientes_small.csv`
- `patient_readings_simulation 1.bsf` (*principal*), `..._small 1.bsf`, `..._tiny 1.bsf`

**Salida** (raíz del proyecto):
- `anomalias.txt` → T/P/O anómalas por paciente (cronológico).
- `pacientes_ecg_anomalos.dat` → formato BIN (ID 11B + N 4B + (fecha 24B + double 8B)*N).
- `validation_ecg_anomalies.txt` → conversión legible del BIN (generada por `validacion`).

---

## 9. Pruebas Realizadas / Sugeridas
- **Archivos tiny/small** permiten validar rápido lectura BSF y reglas de anomalía.
- Comparar `anomalias.txt` con expectativas manuales del CSV/umbrales.
- Probar cambio de BSF (tiny ↔ small) sin tocar el resto del flujo.
- Validar que `pacientes_ecg_anomalos.dat` se convierta con `./validacion`.

---

## 10. Uso de IA y Transparencia
Se utilizó asistencia de IA **solo** para documentación y apoyo de estructuración (README/Informe/Makefile), manteniendo la autoría del código por el equipo.  
**Prompts y enlaces** (a completar por el equipo según lo solicitado por el profesor):
- Prompt 1: _..._
- Prompt 2: _..._
- Conversación/Link: _..._

> Según la guía del curso, se debe **explicar** el código y demostrar comprensión durante la evaluación.

---

## 11. Limitaciones y Trabajo Futuro
- No se usa STL ni contenedores genéricos; el manejo de memoria es manual.
- El ordenamiento por inserción es O(n²); suficiente para volúmenes pequeños/medianos de pruebas.
- `leerArchivoBSF` incluye una **cota** de seguridad para `numLecturas`.  
- Futuro (fuera del alcance del curso): manejo de errores más granular, pruebas automatizadas, parametrización de rutas por CLI.

---

## 12. Conclusión
El sistema cumple los requisitos: lectura de **texto/binario**, **estructuras** dinámicas y detección de anomalías (incluyendo ECG), con generación de **reportes** y **exportación** para verificación. La implementación prioriza claridad, memoria dinámica, y buenas prácticas acordes al nivel del curso.

