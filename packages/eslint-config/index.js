module.exports = {
  root: true,
  env: {
    browser: true,
    es2022: true,
    node: true
  },
  extends: [
    "eslint:recommended",
    "plugin:@typescript-eslint/recommended-type-checked",
    "plugin:@typescript-eslint/stylistic-type-checked",
    "plugin:react-hooks/recommended",
    "plugin:jsx-a11y/recommended",
    "prettier"
  ],
  parser: "@typescript-eslint/parser",
  parserOptions: {
    ecmaVersion: "latest",
    sourceType: "module",
    project: ["./tsconfig.json", "./packages/tsconfig/web.json"],
    tsconfigRootDir: __dirname
  },
  plugins: ["@typescript-eslint", "react-hooks", "jsx-a11y", "import"],
  settings: {
    "import/resolver": {
      typescript: {},
      node: {}
    }
  },
  rules: {
    "@typescript-eslint/no-unused-vars": ["error", { "argsIgnorePattern": "^_" }],
    "@typescript-eslint/no-explicit-any": "warn",
    "@typescript-eslint/consistent-type-imports": "warn",
    "@typescript-eslint/no-floating-promises": "warn",
    "@typescript-eslint/await-thenable": "warn",
    "react-hooks/exhaustive-deps": "warn",
    "import/no-unresolved": "off",
    "import/order": ["warn", { "groups": ["builtin", "external", "internal", "parent", "sibling", "index"], "alphabetize": { "order": "asc" } }],
    "jsx-a11y/anchor-is-valid": "off",
    "jsx-a11y/click-events-have-key-events": "warn",
    "jsx-a11y/no-static-element-interactions": "warn",
    "no-console": ["warn", { "allow": ["warn", "error", "info"] }]
  },
  ignores: ["dist/**", "build/**", "node_modules/**", "**/*.config.ts", "vitest.config.ts", "playwright.config.ts", "**/d.ts"]
}
