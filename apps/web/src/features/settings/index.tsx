import { PageHeader } from '@/shared/components/layout';
import { SettingsIcon, User, Shield, Bell } from 'lucide-react';

export function SettingsPage() {
  return (
    <div>
      <PageHeader title="Configuración" subtitle="Ajustes del sistema y preferencias" />
      
      <div className="grid grid-cols-1 md:grid-cols-3 gap-6">
        <nav className="lg:col-span-1">
          <div className="metric-card p-4">
            <div className="space-y-2">
              <button className="w-full text-left px-3 py-2 text-sm text-neutral-600 hover:bg-neutral-100 hover:text-neutral-900 rounded-md">
                <SettingsIcon className="w-4 h-4 mr-2" /> General
              </button>
              <button className="w-full text-left px-3 py-2 text-sm text-neutral-600 hover:bg-neutral-100 hover:text-neutral-900 rounded-md">
                <User className="w-4 h-4 mr-2" /> Perfil
              </button>
              <button className="w-full text-left px-3 py-2 text-sm text-neutral-600 hover:bg-neutral-100 hover:text-neutral-900 rounded-md">
                <Shield className="w-4 h-4 mr-2" /> Seguridad
              </button>
              <button className="w-full text-left px-3 py-2 text-sm text-neutral-600 hover:bg-neutral-100 hover:text-neutral-900 rounded-md">
                <Bell className="w-4 h-4 mr-2" /> Notificaciones
              </button>
            </div>
          </div>
        </nav>

        <div className="lg:col-span-2">
          <div className="metric-card">
            <h3 className="font-semibold text-neutral-900 mb-6">Configuración General</h3>
            <div className="space-y-4">
              <div>
                <label className="block text-sm font-medium text-neutral-700 mb-1">Unidad de temperatura</label>
                <select className="w-full px-3 py-2 text-sm text-neutral-900 bg-white border border-neutral-300 rounded-md focus:outline-none focus:ring-2 focus:ring-blue-600">
                  <option>Celsius (°C)</option>
                  <option>Fahrenheit (°F)</option>
                </select>
              </div>
              <div>
                <label className="block text-sm font-medium text-neutral-700 mb-1">Intervalo de actualización</label>
                <select className="w-full px-3 py-2 text-sm text-neutral-900 bg-white border border-neutral-300 rounded-md focus:outline-none focus:ring-2 focus:ring-blue-600">
                  <option>30 segundos</option>
                  <option>1 minuto</option>
                  <option>5 minutos</option>
                </select>
              </div>
              <div>
                <label className="block text-sm font-medium text-neutral-700 mb-1">Tema</label>
                <select className="w-full px-3 py-2 text-sm text-neutral-900 bg-white border border-neutral-300 rounded-md focus:outline-none focus:ring-2 focus:ring-blue-600">
                  <option>Sistema</option>
                  <option>Claro</option>
                  <option>Oscuro</option>
                </select>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
}
