import { Link, useLocation } from 'react-router-dom';
import { useState } from 'react';
import { Menu, X, Home, ChartLine, Cloud, Cpu, User } from 'lucide-react';

const navItems = [
  { path: '/', label: 'Dashboard', icon: <Home className="w-5 h-5" /> },
  { path: '/historical', label: 'Histórico', icon: <ChartLine className="w-5 h-5" /> },
  { path: '/device-status', label: 'Dispositivo', icon: <Cpu className="w-5 h-5" /> },
  { path: '/weather', label: 'Clima', icon: <Cloud className="w-5 h-5" /> },
  { path: '/settings', label: 'Configuración', icon: <User className="w-5 h-5" /> }
];

export function Navigation() {
  const location = useLocation();
  const [isOpen, setIsOpen] = useState(false);

  return (
    <nav className="sticky top-0 z-40 bg-white/80 backdrop-blur border-b border-neutral-200">
      <div className="max-w-7xl mx-auto px-4">
        <div className="flex items-center justify-between h-12">
          <div className="flex items-center gap-2">
            <div className="w-8 h-8 rounded-lg bg-blue-600 flex items-center justify-center text-white">
              <span className="text-xs font-bold">S</span>
            </div>
            <span className="font-semibold text-neutral-900 hidden sm:block">Solar Monitoring</span>
          </div>
          
          <div className="flex items-center gap-1">
            <button
              onClick={() => setIsOpen(!isOpen)}
              className="p-2 rounded-md text-neutral-600 hover:bg-neutral-100 sm:hidden"
              aria-label="Menú"
            >
              {isOpen ? <X className="w-5 h-5" /> : <Menu className="w-5 h-5" />}
            </button>
            
            <div className="hidden sm:flex items-center gap-1">
              {navItems.map((item) => {
                const isActive = location.pathname === item.path || location.pathname.startsWith(item.path + '/');
                return (
                  <Link
                    key={item.path}
                    to={item.path}
                    className={`px-3 py-2 text-sm font-medium rounded-md transition-colors ${
                      isActive
                        ? 'bg-blue-600 text-white'
                        : 'text-neutral-600 hover:bg-neutral-100 hover:text-neutral-900'
                    }`}
                  >
                    {item.icon}
                    <span className="ml-2">{item.label}</span>
                  </Link>
                );
              })}
            </div>
          </div>
        </div>
      </div>
      
      {isOpen && (
        <div className="sm:hidden fixed inset-0 z-50 bg-white/95 backdrop-blur">
          <div className="p-6">
            <button onClick={() => setIsOpen(false)} className="mb-4 p-2 rounded-md text-neutral-600 hover:bg-neutral-100">
              <X className="w-5 h-5" />
            </button>
            {navItems.map((item) => {
              const isActive = location.pathname === item.path || location.pathname.startsWith(item.path + '/');
              return (
                <Link
                  key={item.path}
                  to={item.path}
                  onClick={() => setIsOpen(false)}
                  className={`block px-4 py-3 text-sm font-medium rounded-lg transition-colors ${
                    isActive
                      ? 'bg-blue-600 text-white'
                      : 'text-neutral-600 hover:bg-neutral-100 hover:text-neutral-900'
                  }`}
                >
                  {item.icon}
                  <span className="ml-3">{item.label}</span>
                </Link>
              );
            })}
          </div>
        </div>
      )}
    </nav>
  );
}
