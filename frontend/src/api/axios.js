import axios from 'axios';

const instance = axios.create({
  baseURL: 'http://13.58.148.115:5000', 
  withCredentials: true,
  headers: {
    'Content-Type': 'application/json',
  },
});

export default instance;
