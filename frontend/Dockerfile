FROM node:18-alpine
WORKDIR /app
COPY package.json ./
RUN npm i
COPY . .
ARG REACT_APP_BACKEND_URL
ENV REACT_APP_BACKEND_URL=$REACT_APP_BACKEND_URL
RUN npm run build


