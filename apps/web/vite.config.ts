import { defineConfig } from 'vite';
import react from '@vitejs/plugin-react';
import path from 'path';

export default defineConfig({
  plugins: [react()],
  resolve: {
    alias: {
      '@': path.resolve(__dirname, './src'),
      '@solar/ui': path.resolve(__dirname, '../../packages/ui/src'),
      '@solar/design-tokens': path.resolve(__dirname, '../../packages/design-tokens/src'),
      '@solar/firebase-types': path.resolve(__dirname, '../../packages/firebase-types/src')
    }
  },
  server: {
    port: 5173,
    open: true,
    proxy: {
      '/api': {
        target: 'http://localhost:9000',
        changeOrigin: true
      }
    }
  },
  build: {
    outDir: 'dist',
    sourcemap: true,
    rollupOptions: {
      output: {
        manualChunks: {
          vendor: ['react', 'react-dom', '@tanstack/react-query'],
          firebase: ['firebase/app', 'firebase/database', 'firebase/auth'],
          charts: ['recharts'],
          ui: ['clsx', 'tailwind-merge', 'lucide-react']
        }
      }
    }
  },
  test: {
    environment: 'jsdom',
    include: ['src/**/*.test.{ts,tsx}'],
    exclude: ['node_modules', 'dist', 'apps/web/src/**/*.e2e.{ts,tsx}'],
    coverage: {
      provider: 'v8',
      reporter: ['text', 'json', 'html'],
      thresholds: {
        lines: 80,
        branches: 70,
        functions: 80,
        statements: 80
      }
    }
  }
});
