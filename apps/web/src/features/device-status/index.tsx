import { PageHeader } from '@/shared/components/layout';
import { RefreshCw } from 'lucide-react';

export function DeviceStatusPage() {
  return (
    <div>
      <PageHeader title="Estado del Dispositivo" subtitle="Información técnica del ESP32-C3 y conectividad" />
      
      <div className="grid grid-cols-1 md:grid-cols-3 gap-6">
        <div className="metric-card">
          <h3 className="font-semibold text-neutral-900 mb-4">Conectividad</h3>
          <div className="space-y-3">
            <div className="flex items-center justify-between">
              <span className="text-neutral-600">Estado Wi-Fi</span>
              <span className="text-xs font-semibold bg-emerald-100 text-emerald-700 px-2 py-1 rounded">Conectado</span>
            </div>
            <div className="flex items-center justify-between">
              <span className="text-neutral-600">Señal (RSSI)</span>
              <span className="text-neutral-900 font-mono">-67 dBm</span>
            </div>
            <div className="flex items-center justify-between">
              <span className="text-neutral-600">Última sincronización</span>
              <span className="text-neutral-900 font-mono">Hace 30s</span>
            </div>
          </div>
        </div>

        <div className="metric-card">
          <h3 className="font-semibold text-neutral-900 mb-4">Firmware</h3>
          <div className="space-y-3">
            <div className="flex items-center justify-between">
              <span className="text-neutral-600">Versión</span>
              <span className="text-neutral-900 font-mono">v1.0.0</span>
            </div>
            <div className="flex items-center justify-between">
              <span className="text-neutral-600">Uptime</span>
              <span className="text-neutral-900 font-mono">4h 22m</span>
            </div>
            <div className="flex items-center justify-between">
              <span className="text-neutral-600">Memoria libre</span>
              <span className="text-neutral-900 font-mono">185 KB</span>
            </div>
            <button className="w-full btn-ghost mt-4">
              <RefreshCw className="w-4 h-4 mr-2" /> Actualizar firmware
            </button>
          </div>
        </div>

        <div className="metric-card">
          <h3 className="font-semibold text-neutral-900 mb-4">Batería</h3>
          <div className="space-y-3">
            <div className="flex items-center justify-between">
              <span className="text-neutral-600">Voltaje</span>
              <span className="text-neutral-900 font-mono">3.87 V</span>
            </div>
            <div className="flex items-center justify-between">
              <span className="text-neutral-600">Corriente</span>
              <span className="text-neutral-900 font-mono">1.15 A</span>
            </div>
            <div className="flex items-center justify-between">
              <span className="text-neutral-600">SoC estimado</span>
              <span className="text-neutral-900 font-mono">87.5%</span>
            </div>
            <div className="w-full bg-neutral-200 rounded-full h-2 mb-4">
              <div className="bg-emerald-600 h-2 rounded-full" style={{ width: '87.5%' }} />
            </div>
          </div>
        </div>
      </div>
    </div>
  );
}
