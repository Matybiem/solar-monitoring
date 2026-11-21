import { PageHeader, StatCard } from '@/shared/components/layout';
import { Wifi, Battery, Cloud, Monitor, AlertCircle } from 'lucide-react';

export function DashboardPage() {
  return (
    <div>
      <PageHeader title="Dashboard" subtitle="Monitoreo en tiempo real de tu instalación solar" />
      
      <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-4 gap-4 mb-6">
        <StatCard
          title="Generación Solar"
          value="0.0"
          unit="kW"
          trend="+12% vs ayer"
          status="good"
        />
        <StatCard
          title="Batería"
          value="0"
          unit="%"
          trend="Estable"
          status="good"
        />
        <StatCard
          title="Dispositivo"
          value="Online"
          trend="Último: hace 30s"
          status="good"
        />
        <StatCard
          title="Señal Wi-Fi"
          value="-67"
          unit="dBm"
          trend="Buena"
          status="good"
        />
      </div>

      <div className="grid grid-cols-1 lg:grid-cols-3 gap-6">
        <div className="lg:col-span-2">
          <div className="metric-card">
            <div className="flex justify-between items-center mb-4">
              <h3 className="font-semibold text-neutral-900">Generación Solar - Tiempo Real</h3>
              <span className="text-xs font-semibold bg-emerald-100 text-emerald-700 px-2 py-1 rounded">En vivo</span>
            </div>
            <div className="h-64 flex items-center justify-center text-neutral-400">
              <Monitor className="w-12 h-12 opacity-50" />
              <p className="ml-2">Gráfica de generación en tiempo real</p>
            </div>
          </div>
        </div>

        <div>
          <div className="metric-card">
            <h3 className="font-semibold text-neutral-900 mb-4">Estado del Sistema</h3>
            <div className="space-y-3">
              <div className="flex items-center justify-between">
                <div className="flex items-center gap-3">
                  <Wifi className="w-4 h-4 text-blue-500" />
                  <span>Wi-Fi</span>
                </div>
                <span className="text-xs font-semibold bg-emerald-100 text-emerald-700 px-2 py-1 rounded">Conectado</span>
              </div>
              <div className="flex items-center justify-between">
                <div className="flex items-center gap-3">
                  <Battery className="w-4 h-4 text-green-500" />
                  <span>Batería</span>
                </div>
                <span className="text-xs font-semibold text-neutral-600">0%</span>
              </div>
              <div className="flex items-center justify-between">
                <div className="flex items-center gap-3">
                  <Cloud className="w-4 h-4 text-indigo-500" />
                  <span>Nube</span>
                </div>
                <span className="text-xs font-semibold bg-emerald-100 text-emerald-700 px-2 py-1 rounded">Sincronizado</span>
              </div>
              <div className="flex items-center justify-between">
                <div className="flex items-center gap-3">
                  <AlertCircle className="w-4 h-4 text-neutral-400" />
                  <span>Alertas activas</span>
                </div>
                <span className="text-xs font-semibold bg-neutral-100 text-neutral-700 px-2 py-1 rounded">0</span>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
}
