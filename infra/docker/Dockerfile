FROM node:20-alpine

WORKDIR /app

RUN corepack enable && corepack prepare pnpm@9.12.0 --activate

COPY package.json pnpm-lock.yaml* ./
COPY turbo.json ./
COPY packages/ packages/
COPY apps/web/ apps/web/

RUN pnpm install --frozen-lockfile

RUN pnpm --filter @solar/web build

EXPOSE 5173

CMD ["pnpm", "--filter", "@solar/web", "preview"]
