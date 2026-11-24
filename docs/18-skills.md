# Skills de Claude / Claude Skills

**Propósito:** Definir qué son las Skills en el contexto de Claude Code y documentar qué Skills tiene este proyecto Solar Monitoring.

---

## 1. ¿Qué son las Skills?

Las Skills de Claude son **carpetas de instrucciones y recursos reutilizables** (centradas en un archivo `SKILL.md`) que le enseñan a Claude cómo ejecutar flujos de trabajo o tareas específicas de forma consistente, **sin repetir prompts**.

- Una skill es un **procedimiento bajo demanda** ("cómo ejecutar esta tarea").
- Se invocan cuando se necesita (ej.: "desplegar firmware", "correr tests", "revisar PR").
- Se almacenan en `.claude/skills/` (no se ha creado aún en este proyecto).

---

## 2. Skills vs Rules (Distinción)

| Aspecto | Rule (regla) | Skill |
|---------|--------------|-------|
| **Naturaleza** | Contexto normativo | Procedimiento ejecutable |
| **Pregunta** | "¿Qué estándares respetar siempre?" | "¿Cómo ejecutar esta tarea?" |
| **Alcance** | Permanente (siempre aplica) | Bajo demanda (cuando se invoca) |
| **Ejemplo proyecto** | `docs/17-rules.md` (rama, DB, commits) | "Flashear firmware a ESP32-C3" |
| **Archivo clave** | Markdown normativo | `SKILL.md` en su carpeta |

---

## 3. ¿Tenemos Skills ya incluidas bajo otro título?

**Diagnóstico:** No existe una carpeta `.claude/skills/` formal en el repositorio. Sin embargo, **ciertos documentos actúan funcionalmente como skills** porque describen procedimientos bajo demanda:

| Documento actual | ¿Funciona como Skill? | Tipo de contenido |
|------------------|----------------------|-------------------|
| `docs/14-deployment-runbook.md` (secciones 5.x SOPs) | **Sí** — procedimientos ejecutables paso a paso | SOP (Standard Operating Procedure) |
| `docs/06-firmware-design.md` | **Parcial** — describe arquitectura, no pasos | Documentación de diseño |
| `docs/12-testing-strategy.md` | **Parcial** — define qué testear, no cómo ejecutarlo en este proyecto | Estrategia |
| `docs/16-contributing-guide.md` | **Parcial** — lineamientos de contribución | Guía general |

**Conclusión:** Aunque no hay un `.claude/skills/` formal, los **SOPs de la sección 5 de `docs/14-deployment-runbook.md`** cumplen el rol de skills para operaciones (flashear, desplegar, rotar secrets). Si se quisiera formalizar Skills para Claude Code, estos serían los candidatos:

1. **`flash-firmware`** — Flashear ESP32-C3 con `idf.py flash -p COM5` (modo download BOOT).
2. **`monitor-serial`** — Abrir monitor serial y capturar telemetría.
3. **`deploy-firebase-rules`** — Desplegar reglas de seguridad a Firebase.
4. **`build-web`** — Compilar y previsualizar el dashboard web (`pnpm build`, `pnpm preview`).
5. **`rollback-deploy`** — Rollback de Firebase Hosting tras un incidente.

**Si se decides crear skills formales**, se deberá:

1. Crear carpeta `.claude/skills/`
2. Crear un `SKILL.md` por cada skill con: `name`, `description`, `trigger` (cuándo invocar), `steps` (procedimiento detallado).
3. Mantenerlas sincronizadas con `docs/14-deployment-runbook.md`.

---

## 4. Convenciones al crear una Skill

Un `SKILL.md` debe contener:

```markdown
---
name: flash-firmware
description: Flashear firmware compilado a ESP32-C3 en modo download
trigger: ["firmware", "flash", "ESP32"]
---

## Cuándo usar esta skill
- Cuando se necesite cargar nuevo firmware al dispositivo.

## Requisitos previos
- Hardware ESP32-C3 conectado por USB (cable con datos).
- Entorno ESP-IDF configurado (Python, CMake, toolchain en PATH).
- Firmware compilado (`idf.py build` exitoso).

## Procedimiento
1. Mantener presionado BOOT.
2. Conectar USB / pulsar EN/RST.
3. Soltar BOOT cuando COM se estabilice.
4. `idf.py flash -p COM5`
5. `idf.py monitor -p COM5` para verificar arranque.
```

---

## 5. Skills vs. Documentación Tradicional

No reemplazan la documentación existente:

- **Docs (`docs/*.md`)** = conocimiento persistente, contexto, decisiones (`07-decisions-log.md`, `10-project-context.md`).
- **Rules (`docs/17-rules.md`)** = estándares y restricciones permanentes.
- **Skills (`SKILL.md`)** = procedimientos ejecutables, paso a paso, disparados bajo demanda.

---

## Historial de versiones

| Versión | Fecha | Autor | Cambios |
|---------|-------|-------|---------|
| 0.1 | 2026-11-23 | Kilo (AI) | Documento inicial: distinción Skills vs Rules, diagnóstico de skills existentes, convenciones para crear SKILL.md |