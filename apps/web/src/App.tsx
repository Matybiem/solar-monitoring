import { QueryClient, QueryClientProvider } from '@tanstack/react-query';
import { ReactQueryDevtools } from '@tanstack/react-query-devtools';
import { BrowserRouter, Routes, Route } from 'react-router-dom';
import { FirebaseAppProvider } from '@/shared/lib/firebase';
import { ToastProvider } from '@/shared/ui/toast';
import { ThemeToggle } from '@/shared/components/theme-toggle';
import { Navigation } from '@/shared/components/navigation';
import { DashboardPage } from '@/features/dashboard';
import { HistoricalPage } from '@/features/historical';
import { DeviceStatusPage } from '@/features/device-status';
import { WeatherPage } from '@/features/weather';
import { SettingsPage } from '@/features/settings';
import { NotFoundPage } from '@/shared/components/not-found';
import { Layout } from '@/shared/components/layout';
import './styles/index.css';

const queryClient = new QueryClient({
  defaultOptions: {
    queries: {
      staleTime: 1000 * 30,
      gcTime: 1000 * 60 * 5,
      refetchOnWindowFocus: false
    }
  }
});

export default function App() {
  return (
    <QueryClientProvider client={queryClient}>
      <FirebaseAppProvider>
        <ToastProvider>
          <BrowserRouter>
            <div className="min-h-screen flex flex-col bg-neutral-25 text-neutral-900">
              <ThemeToggle />
              <Navigation />
              <main className="flex-1 w-full max-w-7xl mx-auto px-4 py-6">
                <Routes>
                  <Route path="/" element={<Layout><DashboardPage /></Layout>} />
                  <Route path="/historical" element={<Layout><HistoricalPage /></Layout>} />
                  <Route path="/device-status" element={<Layout><DeviceStatusPage /></Layout>} />
                  <Route path="/weather" element={<Layout><WeatherPage /></Layout>} />
                  <Route path="/settings" element={<Layout><SettingsPage /></Layout>} />
                  <Route path="*" element={<NotFoundPage />} />
                </Routes>
              </main>
            </div>
            <ReactQueryDevtools initialIsOpen={false} />
          </BrowserRouter>
        </ToastProvider>
      </FirebaseAppProvider>
    </QueryClientProvider>
  );
}
