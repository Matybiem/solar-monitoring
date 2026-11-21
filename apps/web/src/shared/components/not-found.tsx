export function NotFoundPage() {
  return (
    <div className="flex flex-col items-center justify-center py-20">
      <h1 className="text-6xl font-bold text-neutral-900">404</h1>
      <p className="text-xl text-neutral-600 mt-2">Página no encontrada</p>
      <button
        onClick={() => window.location.href = '/'}
        className="mt-6 btn-primary"
      >
        Volver al inicio
      </button>
    </div>
  );
}
