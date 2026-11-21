import { PageHeader, EmptyState } from '@/shared/components/layout';

export function HistoricalPage() {
  return (
    <div>
      <PageHeader title="Datos Históricos" subtitle="Analiza tendencias de generación y consumo" />
      
      <EmptyState
        title="Sin datos históricos disponibles"
        description="Los datos históricos aparecerán aquí una vez el dispositivo comience a enviar telemetría."
        action={
          <button className="btn-primary">Ver Dashboard</button>
        }
      />
    </div>
  );
}
