import { Children, ReactNode } from 'react';

export function Layout({ children }: { children: ReactNode }) {
  return (
    <div className="space-y-6">
      {Children.map(children, (child) => (
        <div key={child && (child as any).key} className="glass-panel rounded-xl p-6">
          {child}
        </div>
      ))}
    </div>
  );
}

export function PageHeader({ title, subtitle }: { title: string; subtitle?: string }) {
  return (
    <div className="mb-6">
      <h1 className="text-2xl font-bold text-neutral-900">{title}</h1>
      {subtitle && <p className="text-neutral-600 mt-1">{subtitle}</p>}
    </div>
  );
}

export function StatCard({ title, value, unit, trend, status }: { title: string; value: string | number; unit?: string; trend?: string; status?: 'good' | 'warning' | 'danger' | 'neutral' }) {
  const statusColors = {
    good: 'bg-emerald-100 text-emerald-700',
    warning: 'bg-amber-100 text-amber-700',
    danger: 'bg-rose-100 text-rose-700',
    neutral: 'bg-neutral-100 text-neutral-700'
  };

  return (
    <div className="metric-card">
      <div className="flex justify-between items-start mb-3">
        <div>
          <p className="text-sm text-neutral-500">{title}</p>
        </div>
        <span className={`px-2 py-1 rounded-full text-xs font-semibold ${statusColors[status || 'neutral']}`}>
          {status}
        </span>
      </div>
      <div className="flex items-baseline gap-1">
        <span className="text-3xl font-bold text-neutral-900">{value}</span>
        {unit && <span className="text-sm font-medium text-neutral-500">{unit}</span>}
      </div>
      {trend && (
        <p className={`text-xs mt-1 ${trend.startsWith('-') ? 'text-emerald-600' : 'text-rose-600'}`}>
          {trend}
        </p>
      )}
    </div>
  );
}

export function EmptyState({ title, description, action }: { title: string; description: string; action?: React.ReactNode }) {
  return (
    <div className="flex flex-col items-center justify-center py-20 text-neutral-500">
      <div className="w-16 h-16 rounded-full bg-neutral-100 flex items-center justify-center mb-4">
        <span className="text-2xl">📭</span>
      </div>
      <h3 className="text-lg font-medium text-neutral-900">{title}</h3>
      <p className="text-neutral-500 mt-2 max-w-sm text-center">{description}</p>
      {action && <div className="mt-6">{action}</div>}
    </div>
  );
}

export function ToastProvider({ children }: { children: ReactNode }) {
  return <>{children}</>;
}
