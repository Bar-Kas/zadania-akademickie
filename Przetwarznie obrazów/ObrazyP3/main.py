import cv2
import datetime
import time


def main():
    cap = cv2.VideoCapture(0)

    if not cap.isOpened():
        print("Błąd: Nie można otworzyć kamery.")
        return
    cap.set(3, 1280)
    cap.set(4, 720)

    ret, frame_bg = cap.read()
    if not ret:
        print("Błąd odczytu.")
        return

    gray_bg = cv2.cvtColor(frame_bg, cv2.COLOR_BGR2GRAY)
    gray_bg = cv2.GaussianBlur(gray_bg, (21, 21), 0)
    motion_start_time = None
    next_snapshot_time = 0
    min_duration = 5.0
    interval_duration = 5.0
    threshold_value = 25
    show_grayscale = False
    window_name = "Zadanie 12"
    cv2.namedWindow(window_name, cv2.WINDOW_NORMAL)
    cv2.resizeWindow(window_name, 1280, 720)

    while True:
        ret, frame = cap.read()
        if not ret:
            break
        gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        gray_frame = cv2.GaussianBlur(gray_frame, (21, 21), 0)
        if show_grayscale:
            display_frame = cv2.cvtColor(gray_frame, cv2.COLOR_GRAY2BGR)
        else:
            display_frame = frame.copy()
        diff = cv2.absdiff(gray_bg, gray_frame)
        _, thresh = cv2.threshold(diff, threshold_value, 255, cv2.THRESH_BINARY)
        thresh = cv2.dilate(thresh, None, iterations=2)
        contours, _ = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

        motion_detected_now = False

        for contour in contours:
            if cv2.contourArea(contour) < 2000:
                continue

            motion_detected_now = True
            (x, y, w, h) = cv2.boundingRect(contour)
            cv2.rectangle(display_frame, (x, y), (x + w, y + h), (0, 0, 255), 2)
        if motion_detected_now:
            current_timestamp = time.time()
            if motion_start_time is None:
                motion_start_time = current_timestamp
                next_snapshot_time = current_timestamp + min_duration

            elapsed_time = current_timestamp - motion_start_time

            if elapsed_time < min_duration:
                countdown = min_duration - elapsed_time
                msg = f"WERYFIKACJA: {elapsed_time:.1f}s"
                color = (0, 255, 255)
            else:
                msg = "NAGRYWANIE"
                color = (0, 255, 0)

            cv2.putText(display_frame, msg, (20, 40), cv2.FONT_HERSHEY_SIMPLEX, 1.0, color, 2)

            if current_timestamp >= next_snapshot_time:
                filename = f"zad12_{datetime.datetime.now().strftime('%H%M%S')}.jpg"
                cv2.imwrite(filename, frame)

                print(f"Zapisano: {filename}")
                next_snapshot_time = current_timestamp + interval_duration
                cv2.putText(display_frame, "ZAPISANO!", (20, 80), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0, 0, 255), 3)
        else:
            if motion_start_time is not None:
                motion_start_time = None
        status_mode = "GRAYSCALE" if show_grayscale else "COLOR"
        cv2.putText(display_frame, f"Widok: {status_mode}", (20, 660), cv2.FONT_HERSHEY_SIMPLEX, 0.7,
                    (200, 200, 200), 2)
        cv2.putText(display_frame, f"Prog: {threshold_value}", (20, 700), cv2.FONT_HERSHEY_SIMPLEX, 0.7,
                    (255, 255, 255), 2)

        cv2.imshow(window_name, display_frame)

        key = cv2.waitKey(1) & 0xFF

        if key == ord('q'):
            break
        elif key == ord('r'):
            gray_bg = gray_frame
            print("Tło zresetowane.")
        elif key == ord('w'):
            threshold_value += 1
            if threshold_value > 255: threshold_value = 255
        elif key == ord('s'):
            threshold_value -= 1
            if threshold_value < 1: threshold_value = 1
        elif key == ord('g'):
            show_grayscale = not show_grayscale
            print(f"Grayscale: {show_grayscale}")

    cap.release()
    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()