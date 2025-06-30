// import path from 'path'
// import express from 'express'
// import multer from 'multer'
// const router = express.Router()

// const storage = multer.diskStorage({
//   destination(req, file, cb) {
//     cb(null, 'uploads/')
//   },
//   filename(req, file, cb) {
//     cb(
//       null,
//       `${file.fieldname}-${Date.now()}${path.extname(file.originalname)}`
//     )
//   },
// })

// function checkFileType(file, cb) {
//   const filetypes = /jpg|jpeg|png/
//   const extname = filetypes.test(path.extname(file.originalname).toLowerCase())
//   const mimetype = filetypes.test(file.mimetype)

//   if (extname && mimetype) {
//     return cb(null, true)
//   } else {
//     cb('Images only!')
//   }
// }

// const upload = multer({
//   storage,
//   fileFilter: function (req, file, cb) {
//     checkFileType(file, cb)
//   },
// })

// router.post('/', upload.single('image'), (req, res) => {
//   console.log(`${req.file}`);
  
//   res.send(`/${req.file.path}`)
// })



import aws from 'aws-sdk'
import dotenv from 'dotenv'
import express from 'express'
import multer from 'multer'

dotenv.config()
const router = express.Router()

// AWS Config
const region = 'us-east-2'
const bucketName = 'product-image-s3-bucket'
const accessKeyId = process.env.AWS_ACCESS_KEY_ID
const secretAccessKey = process.env.AWS_SECRET_ACCESS_KEY

const s3 = new aws.S3({
  region,
  accessKeyId,
  secretAccessKey,
  signatureVersion: 'v4'
})

// Multer config (memory storage)
const storage = multer.memoryStorage()
const upload = multer({ storage })

// Upload route
router.post('/', upload.single('image'), async (req, res) => {
  try {
    const file = req.file
    if (!file) return res.status(400).send('No file uploaded')

    const fileName = `${Date.now()}_${file.originalname}`

    // Upload to S3
    const uploadParams = {
      Bucket: bucketName,
      Key: fileName,
      Body: file.buffer,
      ContentType: file.mimetype,
    }

    const result = await s3.upload(uploadParams).promise()

    // Return the S3 file URL
    console.log(result.Location);
    
    res.send(result.Location)
  } catch (error) {
    console.error('Error uploading to S3:', error)
    res.status(500).send('Failed to upload file')
  }
})

export default router
