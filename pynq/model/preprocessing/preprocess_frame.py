import numpy as np
import cv2

def preprocess_frame(frame, 
                     image_size=320
                    ):
        
        frame = np.asarray(frame)
        #scaling the image to desired size (padded scaling)
        original_height, original_width = frame.shape[:2]

        scale = min(
            image_size / original_width,
            image_size / original_height
        )

        resized_width = int(round(original_width * scale))
        resized_height = int(round(original_height * scale))

        #resize image with calculated scaling factor
        resized_image = cv2.resize(
            frame,
            (resized_width, resized_height),
            interpolation=cv2.INTER_LINEAR
            )

        resized_image = np.array(resized_image)

        #padding creation
        pad_left = (image_size - resized_width) // 2
        pad_top = (image_size - resized_height) // 2

        canvas = np.full(
            (image_size, image_size, 3),
            fill_value=114,
            dtype=np.uint8
        )

        #pasting the picture to canvas, resoult is resiszed image with padding
        canvas[
            pad_top:pad_top + resized_height,
            pad_left:pad_left + resized_width
        ] = resized_image
        
        image_tensor = canvas.astype(np.float32) / 255.0
        image_tensor = np.transpose(image_tensor, (2, 0, 1))
        image_tensor = np.expand_dims(image_tensor, axis=0)
        
        return image_tensor