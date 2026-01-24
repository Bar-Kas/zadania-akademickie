import cv2
import time
import random
import numpy as np


def draw_pixelated_noise(image, x, y, w, h, block_size=13):
    x_start = max(0, x)
    y_start = max(0, y)
    x_end = min(image.shape[1], x + w)
    y_end = min(image.shape[0], y + h)

    draw_w = x_end - x_start
    draw_h = y_end - y_start

    if draw_w > 0 and draw_h > 0:
        small_w = max(1, draw_w // block_size)
        small_h = max(1, draw_h // block_size)

        noise_grid = np.random.randint(0, 256, (small_h, small_w), dtype=np.uint8)

        noise_scaled = cv2.resize(noise_grid, (draw_w, draw_h), interpolation=cv2.INTER_NEAREST)

        noise_bgr = cv2.cvtColor(noise_scaled, cv2.COLOR_GRAY2BGR)

        image[y_start:y_end, x_start:x_end] = noise_bgr


    cv2.rectangle(image, (x, y), (x + w, y + h), (0, 0, 0), 2)


def main():
    cascade_path = cv2.data.haarcascades + 'haarcascade_frontalface_default.xml'
    face_cascade = cv2.CascadeClassifier(cascade_path)

    cap = cv2.VideoCapture(0)

    if not cap.isOpened():
        print("Error: Could not open camera.")
        return

    print("Camera started. Press 'q' to quit.")

    num_small_squares = 6
    squares_state = []

    for _ in range(num_small_squares):
        squares_state.append({
            'rel_x': 0,
            'rel_y': 0,
            'w_scale': 0.1,
            'h_scale': 0.2,
            'next_update': time.time()
        })

    while True:
        ret, frame = cap.read()
        if not ret:
            break

        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))

        current_time = time.time()

        for (x, y, w, h) in faces:


            main_scale = 0.75
            mw = int(w * main_scale)
            mh = int(h * main_scale)
            mx = int(x + (w - mw) / 2)
            my = int(y + (h - mh) / 2)


            draw_pixelated_noise(frame, mx, my, mw, mh, block_size=15)


            for square in squares_state:


                if current_time > square['next_update']:

                    square['rel_x'] = random.uniform(-0.2, 0.8)
                    square['rel_y'] = random.uniform(-0.2, 0.8)

                    square['w_scale'] = random.uniform(0.1, 0.3)
                    square['h_scale'] = random.uniform(0.1, 0.3)
                    square['next_update'] = current_time + 0.033

                sw = int(mw * square['w_scale'])
                sh = int(mh * square['h_scale'])
                sx = int(mx + (square['rel_x'] * mw))
                sy = int(my + (square['rel_y'] * mh))


                draw_pixelated_noise(frame, sx, sy, sw, sh, block_size=20)

        cv2.imshow('Noise Glitch', frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()