import customtkinter as ctk
import serial

estado_motor = 0
pwm_val = 0

## Configuración del puerto serial
puerto_serial = 'COM6'  # Cambia esto al puerto correcto
baud_rate = 115200 
envio_serial= serial.Serial(puerto_serial,baud_rate)


##FUNCIONES DE LOS BOTONES

def envioTotal():
    global pwm_val, direccion_actual
    pwm = int(pwm_val)  # Convertir el valor del slider a entero
    mensaje = f"{direccion_actual}{int(pwm):03d}\n"
    print(f"Enviando: {mensaje.encode()}")  # Imprimir en consola para depuración
    envio_serial.write(mensaje.encode())  # Enviar el comando al puerto serial
    

def motor_derecha():
    global direccion_actual
    direccion_actual = "D"  # Dirección de giro a la derecha
    label_estado.configure(text="Estado del motor: girando a la derecha")
    envioTotal()
    


def motor_izquierda():
    global direccion_actual
    direccion_actual = "I"  # Dirección de giro a la izquierda
    label_estado.configure(text="Estado del motor: girando a la izquierda")
    envioTotal() 
    

def motor_stop():
    global direccion_actual
    direccion_actual = "S"  # Detener el motor
    label_estado.configure(text="Estado del motor: detenido")
    envioTotal()  # Enviar el comando al puerto serial
    



#FUNCION DEL SLIDER
def slider_value(pwm_value):
    label_slider.configure(text=f"PWM: {int(pwm_value)}")
    global pwm_val
    pwm_val = int(pwm_value)  
    envioTotal() 
    

#FUNCION DE LA VELOCIDAD DEL MOTOR
def velocidad_motor(velocidad):
    label_velocidad.configure(text=f"Velocidad del motor: {int(velocidad)} RPM")
    



##INTERFAZ
ctk.set_appearance_mode("default")  # Modes: "system" (default), "light", "dark"
ctk.set_default_color_theme("dark-blue")

ventana = ctk.CTk()
ventana.geometry("400x450") #ancho x alto
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

#LABEL VELOCIDAD DEL MOTOR
label_velocidad = ctk.CTkLabel(ventana, text="Velocidad del motor: 0")
label_velocidad.pack(pady=20)

ventana.mainloop()




