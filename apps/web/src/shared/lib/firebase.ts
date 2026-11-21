import { initializeApp } from 'firebase/app';
import { getDatabase } from 'firebase/database';
import { getAuth, signInAnonymously } from 'firebase/auth';
import { useEffect, useState, createContext, useContext, ReactNode, createElement } from 'react';

const firebaseConfig = {
  apiKey: import.meta.env.VITE_FIREBASE_API_KEY || 'demo-api-key',
  authDomain: import.meta.env.VITE_FIREBASE_AUTH_DOMAIN || 'solar-mon-prod.firebase.com',
  databaseURL: import.meta.env.VITE_FIREBASE_DATABASE_URL || 'https://solar-mon-prod-default-rtdb.firebaseio.com',
  projectId: import.meta.env.VITE_FIREBASE_PROJECT_ID || 'solar-mon-prod',
  storageBucket: import.meta.env.VITE_FIREBASE_STORAGE_BUCKET || 'solar-mon-prod.appspot.com',
  messagingSenderId: import.meta.env.VITE_FIREBASE_MESSAGING_SENDER_ID || '123456789',
  appId: import.meta.env.VITE_FIREBASE_APP_ID || '1:123456789:web:abcdef'
};

const app = initializeApp(firebaseConfig);
export const database = getDatabase(app);
export const auth = getAuth(app);

const FirebaseAppContext = createContext<{ isReady: boolean; user: any } | null>(null);

export function FirebaseAppProvider({ children }: { children: ReactNode }) {
  const [isReady, setIsReady] = useState(false);
  const [user, setUser] = useState<any>(null);

  useEffect(() => {
    signInAnonymously(auth)
      .then((userCredential) => {
        setUser(userCredential.user);
        setIsReady(true);
      })
      .catch((error) => {
        console.error('Firebase auth error:', error);
        setIsReady(true);
      });
  }, []);

  if (!FirebaseAppContext) {
    return children;
  }
  return createElement(
    FirebaseAppContext.Provider,
    { value: { isReady, user } },
    children
  );
}

export function useFirebase() {
  const context = useContext(FirebaseAppContext);
  if (!context) throw new Error('useFirebase must be used within FirebaseAppProvider');
  return context;
}
