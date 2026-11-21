# UI/UX Design System / Sistema de Diseño

**Estado:** v1.0 — Solar Monitoring

---

## Propósito del documento

Define el **lenguaje visual y componentes reutilizables** para coherencia, velocidad y accesibilidad. Puentes entre Diseño (Figma) e Ingeniería (Storybook + Código). Inspirado en **Fluent Design System** (Microsoft 365 style) — no es copia pixel-perfect, sino adopción de principios y lenguaje visual.

---

## 1. Principios de Diseño (Fluent-inspirado)

| Principio | Descripción |
|-----------|-------------|
| **Luz** | Opacidad, gradientes sutiles, traslúcido (acrylic-like) para profundidad |
| **Profundidad** | Elevación, sombras, capas, z-index para jerarquía visual |
| **Movimiento** | Transiciones suaves, easing, micro-interacciones |
| **Material** | Texturas, bordes redondeados, superficies táctiles |
| **Escala** | Tipografía variable, espaciado consistente, grid 12-col |

---

## 2. Design Tokens (Fuente única de verdad)

> Definidos en `packages/design-tokens` → Style Dictionary → CSS vars, TS, JSON. Sincronizado con Figma (Tokens Studio).

### 2.1 Color (Semántico + Primitivo)
```json
{
  "color": {
    "primitive": {
      "blue": { "50": "#eff6ff", "100": "#dbeafe", "200": "#bfdbfe", "300": "#93c5fd", "400": "#60a5fa", "500": "#3b82f6", "600": "#2563eb", "700": "#1d4ed8", "800": "#1e40af", "900": "#1e3a5f", "950": "#172554" },
      "emerald": { "50": "#ecfdf5", "100": "#d1fae5", "500": "#10b981", "600": "#059669", "900": "#064e3b" },
      "rose": { "50": "#fff1f2", "100": "#ffe4e6", "500": "#f43f5e", "600": "#e11d48", "900": "#881337" },
      "amber": { "50": "#fffbeb", "100": "#fef3c7", "500": "#f59e0b", "600": "#d97706" },
      "neutral": { "0": "#ffffff", "25": "#f8f9fa", "50": "#fafafa", "100": "#f5f5f5", "200": "#e5e5e5", "300": "#d4d4d4", "400": "#a3a3a3", "500": "#737373", "600": "#525252", "700": "#404040", "800": "#262626", "900": "#171717", "950": "#0a0a0a" }
    },
    "semantic": {
      "primary": { "light": "{blue.600}", "dark": "{blue.400}", "contrast": "{neutral.0}" },
      "success": { "light": "{emerald.600}", "dark": "{emerald.400}", "contrast": "{neutral.0}" },
      "danger": { "light": "{rose.600}", "dark": "{rose.400}", "contrast": "{neutral.0}" },
      "warning": { "light": "{amber.600}", "dark": "{amber.400}", "contrast": "{neutral.900}" },
      "background": { "primary": "{neutral.0}", "secondary": "{neutral.25}", "tertiary": "{neutral.100}", "inverse": "{neutral.900}" },
      "surface": { "elevated": "{neutral.0}", "overlay": "rgba({neutral.900}, 0.1)" },
      "text": { "primary": "{neutral.900}", "secondary": "{neutral.600}", "tertiary": "{neutral.400}", "disabled": "{neutral.300}", "inverse": "{neutral.0}" },
      "border": { "light": "{neutral.200}", "dark": "{neutral.700}", "focus": "{blue.500}", "error": "{rose.500}" }
    }
  }
}
```

### 2.2 Espaciado (Spacing Scale - 4px base)
| Token | Valor | Uso |
|-------|-------|-----|
| `space.0` | `0` | Reset |
| `space.1` | `4px` | Micro gap |
| `space.2` | `8px` | Compact gap |
| `space.3` | `12px` | Small gap |
| `space.4` | `16px` | **Base unit** |
| `space.5` | `20px` | Medium gap |
| `space.6` | `24px` | Large gap |
| `space.8` | `32px` | Section gap |
| `space.10` | `40px` | Layout gap |
| `space.12` | `48px` | Major section |
| `space.16` | `64px` | Hero / Page |

### 2.3 Tipografía
| Token | Font Family | Size | Weight | Line Height | Letter Spacing |
|-------|-------------|------|--------|-------------|----------------|
| `display.xl` | `Inter Variable` | `clamp(2.5rem, 6vw, 4rem)` | `700` | `1.1` | `-0.02em` |
| `heading.xl` | `Inter Variable` | `clamp(2rem, 5vw, 3rem)` | `700` | `1.15` | `-0.01em` |
| `heading.lg` | `Inter Variable` | `clamp(1.5rem, 3vw, 2.25rem)` | `600` | `1.2` | `-0.01em` |
| `heading.md` | `Inter Variable` | `1.25rem` | `600` | `1.3` | `0` |
| `heading.sm` | `Inter Variable` | `1rem` | `600` | `1.4` | `0` |
| `body.lg` | `Inter Variable` | `1.125rem` | `400` | `1.6` | `0` |
| `body.md` | `Inter Variable` | `1rem` | `400` | `1.5` | `0` |
| `body.sm` | `Inter Variable` | `0.875rem` | `400` | `1.5` | `0` |
| `caption.md` | `Inter Variable` | `0.875rem` | `500` | `1.4` | `0.01em` |
| `caption.sm` | `Inter Variable` | `0.75rem` | `500` | `1.4` | `0.02em` |
| `code` | `JetBrains Mono Variable` | `0.875rem` | `400` | `1.6` | `0` |

### 2.4 Sombras / Elevación
| Token | Valor (Light) | Valor (Dark) | Uso |
|-------|---------------|--------------|-----|
| `elevation.0` | `none` | `none` | Flat |
| `elevation.1` | `0 1px 2px rgb(0 0 0 / 0.05)` | `0 1px 2px rgb(0 0 0 / 0.3)` | Cards, Inputs |
| `elevation.2` | `0 4px 6px -1px rgb(0 0 0 / 0.1)` | `0 4px 6px -1px rgb(0 0 0 / 0.4)` | Dropdowns, Tooltips |
| `elevation.3` | `0 10px 15px -3px rgb(0 0 0 / 0.1)` | `0 10px 15px -3px rgb(0 0 0 / 0.4)` | Modals, Drawers |
| `elevation.4` | `0 20px 25px -5px rgb(0 0 0 / 0.1)` | `0 20px 25px -5px rgb(0 0 0 / 0.4)` | Full-screen modals |

### 2.5 Border Radius
| Token | Valor | Uso |
|-------|-------|-----|
| `radius.none` | `0` | Tablas, badges |
| `radius.sm` | `4px` | Inputs, buttons pequeños |
| `radius.md` | `8px` | Cards, dropdowns |
| `radius.lg` | `12px` | Modals, sheets |
| `radius.xl` | `16px` | Hero cards |
| `radius.full` | `9999px` | Pills, avatars |

### 2.6 Breakpoints y Grid 12-col
> **Destacado para README:** Sistema de grid 12 columnas. 12 es mínimo común múltiplo de 2, 3 y 4, lo que permite layouts flexibles (mitad, tercio, cuarto).

| Token | Valor | Columnas | Target |
|-------|-------|----------|--------|
| `bp.sm` | `640px` | 4 | Mobile |
| `bp.md` | `768px` | 8 | Tablet |
| `bp.lg` | `1024px` | 12 | Desktop |
| `bp.xl` | `1280px` | 12 | Desktop grande |
| `bp.2xl` | `1536px` | 12 | Desktop extra grande |

**Reglas de uso del grid:**
- **Desktop (≥1024px):** 12 columnas disponibles — sidebar + contenido principal + widgets
- **Tablet (768-1023px):** 8 columnas — sidebar colapsable, contenido adaptado
- **Mobile (<768px):** 4 columnas — stack vertical, navegación bottom/drawer

### 2.7 Z-Index Scale
| Token | Valor | Capa |
|-------|-------|------|
| `z.hide` | `-1` | Detrás |
| `z.base` | `0` | Default |
| `z.dropdown` | `100` | Dropdowns, Popovers |
| `z.sticky` | `200` | Sticky headers |
| `z.modal` | `300` | Modals, Drawers |
| `z.toast` | `400` | Toasts, Notificaciones |
| `z.tooltip` | `500` | Tooltips (sobre todo) |

### 2.8 Transiciones / Motion
| Token | Duración | Easing | Uso |
|-------|----------|--------|-----|
| `motion.instant` | `0ms` | `linear` | State changes inmediatos |
| `motion.fast` | `150ms` | `cubic-bezier(0.4, 0, 0.2, 1)` | Hover, Focus |
| `motion.normal` | `200ms` | `cubic-bezier(0.4, 0, 0.2, 1)` | Transiciones estándar |
| `motion.slow` | `300ms` | `cubic-bezier(0.4, 0, 0.2, 1)` | Modals, Drawers |
| `motion.reduced` | `0.01ms` | `linear` | `prefers-reduced-motion: reduce` |

---

## 3. Componentes Fundamentales (Atoms → Molecules → Organisms)

### 3.1 Atoms
| Componente | Props clave | Variantes | Estados | A11y |
|------------|-------------|-----------|---------|------|
| `Button` | `variant: primary\|secondary\|ghost\|danger`, `size: sm\|md\|lg`, `loading`, `icon` | 4×3 | `default`, `hover`, `focus`, `active`, `disabled`, `loading` | `role=button`, `aria-disabled`, `aria-busy` |
| `Input` | `type`, `label`, `error`, `hint`, `icon`, `disabled`, `required` | `text`, `email`, `password`, `search`, `number` | `default`, `focus`, `error`, `disabled`, `filled` | `label`, `aria-describedby`, `aria-invalid` |
| `Label` | `required`, `htmlFor` | — | — | `for` binding |
| `Icon` | `name` (lucide), `size`, `aria-hidden` | 200+ icons | — | `aria-hidden=true` default |
| `Badge` | `variant: neutral\|primary\|success\|danger\|warning`, `dot` | 5 | — | `role=status` si dinámico |
| `Avatar` | `src`, `alt`, `fallback` (initials), `size`, `shape` | `circle`, `square` | `loading`, `error` | `alt=""` si decorativo |
| `Spinner` | `size`, `label` (sr-only) | — | — | `role=status`, `aria-live=polite` |
| `Card` | `variant: elevated\|outlined\|filled`, `padding` | 3 | `hover`, `active` | `role=region` si tiene heading |

### 3.2 Molecules
| Componente | Composición | Props clave |
|------------|-------------|-------------|
| `FormField` | `Label` + `Input` + `Error/Hint` | `name`, `validation` (Zod), `required` |
| `Select` | `Button` + `Popover` + `Listbox` + `Option` | `options`, `multiple`, `searchable` |
| `MetricCard` | `Card` + `Icon` + `Number` + `Label` + `Trend` | `value`, `unit`, `trend`, `variant` |
| `LiveBadge` | `Badge` + pulsing animation | `status: online\|offline\|degraded` | `aria-live=polite` |
| `ChartCard` | `Card` + `Chart` (Recharts) + `Controls` + `Legend` | `type: line\|bar`, `data`, `height` |
| `DateRangePicker` | `Input` + `Popover` + `Calendar` | `start`, `end`, `onChange` |

### 3.3 Organisms
| Componente | Descripción |
|------------|-------------|
| `NavigationBar` | Logo, Nav links, User menu, Mobile drawer (Fluent style) |
| `Sidebar` | Collapsible, nested routes, active state, icons |
| `DashboardGrid` | Layout con Grid 12-col: Desktop 12, Tablet 8, Mobile 4 |
| `DataTable` | Table + Toolbar (search, filters, export) + Pagination |
| `FormWizard` | Stepper + FormField + Validation + Progress |
| `SettingsPanel` | Tabs + FormFields + Save/Cancel + Dirty tracking |
| `WeatherWidget` | Card con datos clima, cache indicator, fallback graceful |

---

## 4. Patrones de UX (UX Patterns)

### 4.1 Formularios
- **Validación:** En `blur` + `submit` (no en `change`)
- **Errores:** Debajo del input, color danger semántico, `aria-invalid`, `aria-describedby`
- **Submit:** Deshabilitado mientras `invalid` o `submitting`; spinner en botón
- **Success:** Toast + redirect o modal de confirmación

### 4.2 Tablas de Datos
- **Columnas:** Resizable, reorderable, hideable, pinnable
- **Ordenación:** Click header → `asc/desc/none` (3 estados)
- **Filtros:** Toolbar superior: search global + column filters (faceted)
- **Selección:** Checkbox header (select all visible / all pages)
- **Empty State:** Ilustración + copy acciónable + CTA principal
- **Loading:** Skeleton rows (5-10) + shimmer

### 4.3 Navegación
- **Mobile:** Drawer lateral (hamburger), bottom tab bar opcional
- **Desktop:** Top bar + Sidebar colapsable
- **Breadcrumbs:** Siempre en páginas profundas (> 2 niveles)
- **Active state:** Visible, alto contraste, `aria-current="page"`

### 4.4 Feedback & Loading
| Patrón | Cuándo | Componente |
|--------|--------|------------|
| **Skeleton** | Primer carga de contenido | `Skeleton` (match layout) |
| **Spinner** | Acciones rápidas (< 2s) | `Button loading`, `Spinner` |
| **Progress Bar** | Operaciones largas (> 5s) | `Progress` (determinate/indeterminate) |
| **Optimistic UI** | Mutaciones simples (toggle, like) | React Query `onMutate` + rollback |
| **Toast** | Success/Info/Warning no bloqueantes | `ToastProvider` + `toast()` |
| **Modal** | Confirmaciones destructivas, flujos multi-paso | `Modal` + `FocusTrap` |

---

## 5. Accesibilidad (A11y) - Checklist Obligatorio

| Criterio (WCAG 2.1 AA) | Implementación | Test |
|------------------------|----------------|------|
| **1.1.1 Non-text Content** | `alt` en imágenes, `aria-label` en iconos | axe, manual |
| **1.3.1 Info & Relationships** | HTML semántico (`header`, `nav`, `main`, `section`, `article`, `aside`, `footer`), `label`/`fieldset` | axe |
| **1.4.3 Contrast (Minimum)** | Tokens semánticos garantizan 4.5:1 (texto), 3:1 (UI) | `storybook-addon-a11y`, Lighthouse |
| **1.4.4 Resize Text** | `rem` units, no `px` fijos, zoom hasta 200% | Manual |
| **2.1.1 Keyboard** | Todos los interactivos: `Tab`, `Enter`, `Space`, `Escape`, `Arrows` | Manual + `tabbable` |
| **2.1.2 No Keyboard Trap** | `FocusTrap` en Modals/Drawers | Manual |
| **2.4.3 Focus Order** | DOM order = visual order | Manual |
| **2.4.7 Focus Visible** | `focus-visible` ring (token `border.focus`) | axe, manual |
| **3.3.2 Labels/Instructions** | `Label` visible + `aria-describedby` para hints/errors | axe |
| **4.1.2 Name, Role, Value** | Componentes custom: `role`, `aria-*`, `aria-live` para dinámicos | axe, `testing-library` |

**Herramientas CI:** `axe-core` en Playwright E2E + `storybook-addon-a11y` en PR.

---

## 6. Internacionalización (i18n) & RTL

| Aspecto | Implementación |
|---------|----------------|
| **Strings** | `next-intl` / `react-i18next` — JSON por locale en `packages/ui/locales` |
| **Pluralización** | ICU MessageFormat |
| **Fechas/Números** | `Intl.DateTimeFormat`, `Intl.NumberFormat` (locale del usuario) |
| **RTL** | `dir="rtl"` en `html`, CSS Logical Properties (`margin-inline-start`) |
| **Fuentes** | Variable fonts con fallback `system-ui` |
| **Expansión texto** | Contenedores flexibles, `min-width` no `width` fijo |

---

## 7. Theming (Light / Dark / Brand)

```css
:root {
  --color-primary: var(--color-blue-600);
  --color-background-primary: var(--color-neutral-0);
  --color-text-primary: var(--color-neutral-900);
}

[data-theme="dark"] {
  --color-primary: var(--color-blue-400);
  --color-background-primary: var(--color-neutral-950);
  --color-text-primary: var(--color-neutral-0);
}
```

- **Switcher:** `ThemeProvider` + `localStorage` + `prefers-color-scheme`
- **Sin flash:** Inline script en `<head>` antes de React hydration

---

## 8. Storybook (Documentación Viva)

| Sección | Contenido |
|---------|-----------|
| **Introduction** | Design principles, tokens reference, getting started |
| **Foundations** | Color, Spacing, Typography, Shadows, Breakpoints, Motion, Z-index, Grid 12-col |
| **Components** | Cada componente: Canvas, Docs, Tests, A11y |
| **Patterns** | Formularios, Tablas, Navegación, Feedback, Loading |
| **Templates** | Layouts: Dashboard, Settings, Auth, Empty States |
| **Pages** | Pantallas completas compuestas |

**CI:** Chromatic para visual regression + `storybook-test-runner` para interaction tests.

---

## 9. Métricas de Calidad del Design System

| Métrica | Target | Herramienta |
|---------|--------|-------------|
| **Component Coverage** | 100% componentes en Storybook | Chromatic |
| **A11y Score** | 0 violations (AA) | axe-core CI |
| **Bundle Size Impact** | < 5KB gzipped por componente | `bundle-analyzer` |
| **Grid Compliance** | Desktop 12 / Tablet 8 / Mobile 4 | Visual regression + manual |

---

## Historial de versiones

| Versión | Fecha | Autor | Cambios |
|---------|-------|-------|---------|
| 0.1 | 2024-11-20 | Matías Biem | Plantilla base |
| 1.0 | 2024-11-20 | Matías Biem | Design System Solar Monitoring v1.0: Fluent tokens, Grid 12-col, componentes, a11y, Storybook |