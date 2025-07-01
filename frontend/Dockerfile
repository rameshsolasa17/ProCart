# Stage 1: Build React app
FROM node:18-alpine as builder

WORKDIR /app

# Install dependencies
COPY package.json package-lock.json ./
RUN npm install

# Copy full app source
COPY . .

# Set environment variables
ARG REACT_APP_BACKEND_URL
ENV REACT_APP_BACKEND_URL=$REACT_APP_BACKEND_URL

# Fix OpenSSL issue with Webpack
ENV NODE_OPTIONS=--openssl-legacy-provider

# Build React app
RUN npm run build

# Optional: use a second stage if you want a clean final image for serving
# FROM nginx:alpine
# COPY --from=builder /app/build /usr/share/nginx/html
