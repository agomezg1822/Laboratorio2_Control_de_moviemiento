import customtkinter as ctk
import serial

estado_motor = 0
pwm_val = 0

## Configuración del puerto serial
puerto_serial = 'COM6'  # Cambia esto al puerto correcto
baud_rate = 115200 
envio_serial= serial.Serial(puerto_serial,baud_rate)


##FUNCIONES DE LOS BOTONES

def motor_derecha():
    label_estado.configure(text="Estado del motor: girando a la derecha")
    serial = (f"D\n".encode()) #D de derecha
    envio_serial.write(serial) 
    serial = pwm_val
    print(f"Motor girando a la derecha: {serial}")
    envio_serial.write(serial)  # Enviar el comando al puerto serial


def motor_izquierda():
    label_estado.configure(text="Estado del motor: girando a la izquierda")
    serial = (f"I\n".encode()) #I de izquierda
    envio_serial.write(serial) 
    print(f"Motor girando a la izquierda: {serial}")
    serial = pwm_val
    print(serial)
    envio_serial.write(serial)  # Enviar el comando al puerto serial

def motor_stop():
    label_estado.configure(text="Estado del motor: detenido")
    serial = (f"S\n".encode()) #S de stop
    print(f"Motor detenido: {serial}")
    envio_serial.write(serial)  # Enviar el comando al puerto serial

def slider_value(pwm_value):
    label_slider.configure(text=f"PWM: {int(pwm_value)}")
    serial = (f"{int(pwm_value)}\n".encode())  
    pwm_val = pwm_value  # Convertir el valor del slider a entero
    #print(f"Enviando PWM: {serial}") 
    print(f"Enviando PWM: {pwm_val}")
    envio_serial.write(serial)  # Enviar el valor PWM al puerto serial



##INTERFAZ
ctk.set_appearance_mode("default")  # Modes: "system" (default), "light", "dark"
ctk.set_default_color_theme("dark-blue")

ventana = ctk.CTk()
ventana.geometry("400x400") #ancho x alto
ventana.title("Laboratorio 2")

##BOTONES
boton_derecha = ctk.CTkButton(ventana, text="Botón derecho", command=motor_derecha)
boton_derecha.pack(pady=20)

boton_izquierdo = ctk.CTkButton(ventana, text="Botón izquierdo", command=motor_izquierda)
boton_izquierdo.pack(pady=20)

boton_stop = ctk.CTkButton(ventana, text="Botón stop", command=motor_stop)
boton_stop.pack(pady=20)

##SLIDER
slider = ctk.CTkSlider(ventana, from_=0, to=255, command=slider_value)
slider.pack(pady=20)
label_slider = ctk.CTkLabel(ventana, text="PWM: 0")
label_slider.pack()

##LABEL ESTADO DEL MOTOR
label_estado = ctk.CTkLabel(ventana, text="Estado del motor: detenido")
label_estado.pack(pady=20)

ventana.mainloop()




