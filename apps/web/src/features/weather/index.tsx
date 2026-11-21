import { PageHeader, EmptyState } from '@/shared/components/layout';
import { Cloud, Thermometer, Wind } from 'lucide-react';

export function WeatherPage() {
  return (
    <div>
      <PageHeader title="Información Meteorológica" subtitle="Datos climáticos para análisis de generación solar" />
      
      <div className="grid grid-cols-1 md:grid-cols-4 gap-6">
        <div className="metric-card">
          <h3 className="font-semibold text-neutral-900 mb-2">Temperatura</h3>
          <div className="text-3xl font-bold text-neutral-900">--°C</div>
          <Cloud className="w-8 h-8 text-neutral-400 mt-2" />
        </div>
        <div className="metric-card">
          <h3 className="font-semibold text-neutral-900 mb-2">Humedad</h3>
          <div className="text-3xl font-bold text-neutral-900">--%</div>
          <Thermometer className="w-8 h-8 text-neutral-400 mt-2" />
        </div>
        <div className="metric-card">
          <h3 className="font-semibold text-neutral-900 mb-2">Nubosidad</h3>
          <div className="text-3xl font-bold text-neutral-900">--%</div>
          <Wind className="w-8 h-8 text-neutral-400 mt-2" />
        </div>
        <div className="metric-card">
          <h3 className="font-semibold text-neutral-900 mb-2">Índice UV</h3>
          <div className="text-3xl font-bold text-neutral-900">--</div>
        </div>
      </div>

      <EmptyState
        title="Datos meteorológicos no disponibles"
        description="La integración con el servicio meteorológico estará disponible próximamente."
      />
    </div>
  );
}
