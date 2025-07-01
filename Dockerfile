FROM node:alpine3.18
WORKDIR /app
COPY package.json ./
RUN npm i
COPY backend .
EXPOSE 5000
CMD ["npm", "run", "server"]
