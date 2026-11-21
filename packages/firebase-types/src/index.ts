// packages/firebase-types/src/telemetry.ts
export interface Telemetry {
  schema_version: 1;
  deviceId: string;
  timestamp: number;
  solar_voltage: number;
  solar_current: number;
  solar_power: number;
  battery_voltage: number;
  battery_current: number;
  battery_soc_est: number;
  rssi: number;
  fw_version: string;
  uptime_sec?: number;
  free_heap_bytes?: number;
  wake_reason?: 'timer' | 'external' | 'touch' | 'ulp';
}

export interface Device {
  deviceId: string;
  name: string;
  location?: string;
  fw_version: string;
  hw_version?: string;
  status: 'online' | 'offline' | 'degraded' | 'maintenance';
  last_seen: number;
  battery_health?: 'good' | 'fair' | 'poor' | 'critical';
  config: DeviceConfig;
  created_at: number;
  updated_at: number;
}

export interface DeviceConfig {
  measurement_interval_sec: number;
  deep_sleep_duration_sec: number;
  solar_sensor_addr: string;
  battery_sensor_addr: string;
  soc_estimation_method: 'hybrid_ocv_coulomb';
  soc_calibration: {
    voltage_full: number;
    voltage_empty: number;
    capacity_mah: number;
  };
  thresholds: {
    battery_voltage_low: number;
    battery_voltage_critical: number;
    solar_power_min: number;
    rssi_warning: number;
  };
  wifi: {
    ssid: string;
    reconnect_max_attempts: number;
    reconnect_backoff_base_sec: number;
  };
}

export interface WeatherCache {
  locationKey: string;
  data: WeatherData;
  fetched_at: number;
  expires_at: number;
}

export interface WeatherData {
  current: {
    temp: number;
    humidity: number;
    clouds: number;
    uvi: number;
    wind_speed: number;
    weather: Array<{
      main: string;
      description: string;
      icon: string;
    }>;
  };
  hourly: Array<{
    dt: number;
    temp: number;
    clouds: number;
    uvi: number;
  }>;
  daily: Array<{
    dt: number;
    temp: {
      min: number;
      max: number;
    };
    clouds: number;
    uvi: number;
  }>;
}

export interface AggregatedDaily {
  deviceId: string;
  date: string;
  energy_generated_wh: number;
  energy_consumed_wh: number;
  peak_power_w: number;
  avg_soc: number;
  min_soc: number;
  max_soc: number;
  total_measurements: number;
  uptime_percent: number;
}

export interface FirebaseRules {
  telemetry: {
    read: boolean;
    write: string;
  };
  devices: {
    read: boolean;
    write: string;
  };
}

export * from './telemetry';
