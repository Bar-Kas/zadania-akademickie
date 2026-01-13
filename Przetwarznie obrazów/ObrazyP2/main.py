import cv2
import datetime
import time


def main():
    cap = cv2.VideoCapture(0)

    if not cap.isOpened():
        print("Błąd: Nie można otworzyć kamery")
        return
    cap.set(3, 1280)
    cap.set(4, 720)
    ret, first_frame = cap.read()
    if not ret:
        print("Błąd odczytu z kamery.")
        return

    cv2.namedWindow("Wybierz obszar", cv2.WINDOW_NORMAL)
    cv2.resizeWindow("Wybierz obszar", 1280, 720)

    roi_rect = cv2.selectROI("Wybierz obszar", first_frame, showCrosshair=True, fromCenter=False)
    cv2.destroyWindow("Wybierz obszar")

    roi_x, roi_y, roi_w, roi_h = roi_rect

    if roi_w == 0 or roi_h == 0:
        print("Nie wybrano obszaru. Zamykanie.")
        cap.release()
        return
    roi1 = first_frame[roi_y:roi_y + roi_h, roi_x:roi_x + roi_w]
    gray1 = cv2.cvtColor(roi1, cv2.COLOR_BGR2GRAY)
    gray1 = cv2.GaussianBlur(gray1, (21, 21), 0)
    motion_start_time = None
    next_snapshot_time = 0
    min_duration = 5.0
    interval_duration = 5.0

    threshold_value = 25
    window_name = "Zadanie 13"
    cv2.namedWindow(window_name, cv2.WINDOW_NORMAL)
    cv2.resizeWindow(window_name, 1280, 720)

    print(f"Monitorowany obszar: x={roi_x}, y={roi_y}, w={roi_w}, h={roi_h}")
    print("[W/S] - Czułość | [R] - Reset Tła | [Q] - Koniec")

    while True:
        ret, frame = cap.read()
        if not ret:
            break
        display_frame = frame.copy()
        roi2 = frame[roi_y:roi_y + roi_h, roi_x:roi_x + roi_w]
        gray2 = cv2.cvtColor(roi2, cv2.COLOR_BGR2GRAY)
        gray2 = cv2.GaussianBlur(gray2, (21, 21), 0)
        diff = cv2.absdiff(gray1, gray2)
        _, thresh = cv2.threshold(diff, threshold_value, 255, cv2.THRESH_BINARY)
        thresh = cv2.dilate(thresh, None, iterations=2)

        contours, _ = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

        motion_detected_now = False
        for contour in contours:
            if cv2.contourArea(contour) < 1000:
                continue

            motion_detected_now = True
            (x, y, w, h) = cv2.boundingRect(contour)
            cv2.rectangle(display_frame, (x + roi_x, y + roi_y),
                          (x + roi_x + w, y + roi_y + h), (0, 0, 255), 2)
        if motion_detected_now:
            current_timestamp = time.time()

            if motion_start_time is None:
                motion_start_time = current_timestamp
                next_snapshot_time = current_timestamp + min_duration

            elapsed_time = current_timestamp - motion_start_time
            if elapsed_time < min_duration:
                countdown = min_duration - elapsed_time
                msg = f"Weryfikacja: {countdown:.1f}s"
                color = (0, 255, 255)
            else:
                msg = "NAGRYWANIE"
                color = (0, 255, 0)

            cv2.putText(display_frame, msg, (roi_x, roi_y - 10),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.6, color, 2)
            if current_timestamp >= next_snapshot_time:
                ts_str = datetime.datetime.now().strftime('%H%M%S')
                filename = f"zad13_{ts_str}.jpg"

                cv2.imwrite(filename, frame)
                print(f"[ZAPIS] Ruch w ROI > 5s. Plik: {filename}")

                next_snapshot_time = current_timestamp + interval_duration
                cv2.putText(display_frame, "ZAPISANO!", (roi_x, roi_y + roi_h + 30),
                            cv2.FONT_HERSHEY_SIMPLEX, 1.0, (0, 0, 255), 3)
        else:
            if motion_start_time is not None:
                motion_start_time = None
        cv2.rectangle(display_frame, (roi_x, roi_y), (roi_x + roi_w, roi_y + roi_h), (255, 0, 0), 2)
        cv2.putText(display_frame, f"Czulosc : {threshold_value}", (10, 700), cv2.FONT_HERSHEY_SIMPLEX, 0.7,
                    (255, 255, 255), 2)

        cv2.imshow(window_name, display_frame)
        key = cv2.waitKey(10) & 0xFF

        if cv2.getWindowProperty(window_name, cv2.WND_PROP_VISIBLE) < 1:
            break

        if key == ord('q'):
            print("Koniec programu (klawisz Q).")
            break

        elif key == ord('r'):
            roi1 = frame[roi_y:roi_y + roi_h, roi_x:roi_x + roi_w]
            gray1 = cv2.cvtColor(roi1, cv2.COLOR_BGR2GRAY)
            gray1 = cv2.GaussianBlur(gray1, (21, 21), 0)
            print("Tło ROI zresetowane.")

        elif key == ord('w'):
            threshold_value = min(255, threshold_value + 1)

        elif key == ord('s'):
            threshold_value = max(1, threshold_value - 1)

    cap.release()
    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()