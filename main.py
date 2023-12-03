import tkinter as tk
from tkinter import filedialog
from tkinter import ttk
from tkinter import messagebox
import time
import ctypes
import os
import threading

deal = ctypes.CDLL("deal.dll", winmode = 0)
luc = ctypes.CDLL("luc.dll", winmode = 0)

deal.GenerateKeys(256)

def reset_fields():
    file_entry.delete(0, tk.END)

def process_file():
    progress = tk.DoubleVar()
    progress.set(0)
    progress_bar = ttk.Progressbar(frame1, variable=progress, maximum=100)
    progress_bar.grid(row=11, columnspan=10)
    max_progress = 100
    cipher_mode = 0
    selected_file_path = file_entry.get()
    selected_method = method_choice.get()
    selected_mode = mode_choice.get()
    selected_cipher_mode = cipher_mode_choice.get()
    output_directory = output_entry.get()
    progress.set(10)
    if (selected_mode == "Шифрование"):
        mode = 1
    if (selected_mode == "Дешифрование"):
        mode = 0
    file = selected_file_path.encode('cp866')
    directorytosave = output_directory.encode('cp866')
    progress.set(30)
    time.sleep(0.6)
    if (selected_method == "DEAL"):
        deal.Encryption(file, directorytosave, 256, mode, cipher_mode)
    else:
        luc.Encryption(file, directorytosave, mode)
    progress.set(80)

    if not selected_file_path or not output_directory:
        messagebox.showerror("Ошибка", "Выберите файл и директорию для сохранения.")
        return
    progress.set(90)

    if delete_file_var.get() and os.path.exists(selected_file_path):
        os.remove(selected_file_path)
    progress.set(100)

    messagebox.showinfo("Завершено", "Действие выполнено успешно!")
    f = threading.Thread(target=reset_fields)
    progress.set(0)
    f.start()
    progress_bar.destroy()

def start_processing():
    t = threading.Thread(target=process_file)
    t.start()

def on_select1(event):
    value = method_choice.get()
    if value == "DEAL":
        cipher_mode_choice.state(["!disabled"])
    else:
        cipher_mode_choice.state(["disabled"])

def check_file(*args):
    file_name = file_var.get()
    file2_name = file2_var.get()
    if file_name and file2_name:
        next_button.config(state="normal")
    else:
        next_button.config(state="disabled")

app = tk.Tk()
app.resizable(False, False)
app.iconphoto(False, tk.PhotoImage(file="icon.png"))
app.title("EncryptionApp")

frame1 = ttk.Frame(app)
frame1.pack()

delete_file_var = tk.BooleanVar()
delete_file_var.set(False)

delete_file_checkbutton = ttk.Checkbutton(frame1, text="Удалить подаваемый файл", variable=delete_file_var)
delete_file_checkbutton.grid(row=5, column=0, columnspan=3, pady=10)


file_label = ttk.Label(frame1, text="Выберите файл:")
file_label.grid(row=0, column=0, pady=10)

file_var = tk.StringVar()
file_entry = ttk.Entry(frame1, width=50, textvariable=file_var)
file_entry.grid(row=0, column=1)

browse_file_button = ttk.Button(frame1, text="Обзор", command=lambda: file_var.set(filedialog.askopenfilename()))
browse_file_button.grid(row=0, column=2, padx=10)

method_label = ttk.Label(frame1, text="Выберите метод:")
method_label.grid(row=1, column=0, pady=10)

method_choice = ttk.Combobox(frame1, state="readonly", values=["DEAL", "LUC"])
method_choice.current(0)
method_choice.grid(row=1, column=1)

mode_label = ttk.Label(frame1, text="Выберите режим:")
mode_label.grid(row=3, column=0, pady=10)

mode_choice = ttk.Combobox(frame1, state="readonly", values=["Шифрование", "Дешифрование"])
mode_choice.current(0)
mode_choice.grid(row=3, column=1)

cipher_mode_label = ttk.Label(frame1, text="Выберите режим сцепления:")
cipher_mode_label.grid(row=2, column=0, pady=10)

cipher_mode_choice = ttk.Combobox(frame1, state="readonly", values=["ECB", "CBC", "CFB", "OFB"])
cipher_mode_choice.current(0)
cipher_mode_choice.grid(row=2, column=1)

method_choice.bind("<<ComboboxSelected>>", on_select1)

output_label = ttk.Label(frame1, text="Выберите директорию для сохранения файла:")
output_label.grid(row=4, column=0, pady=10, padx=10)

file2_var = tk.StringVar()
output_entry = ttk.Entry(frame1, width=50, textvariable=file2_var)
output_entry.insert(0, "C:\\Users\\t8met\\Desktop")
output_entry.grid(row=4, column=1)

browse_output_button = ttk.Button(frame1, text="Обзор", command=lambda: file2_var.set(filedialog.askdirectory(title="Выберите директорию для сохранения файла")))
browse_output_button.grid(row=4, column=2, padx=10)



next_button = ttk.Button(frame1, state="disabled", text="Ок", command=start_processing)
next_button.grid(row=6, columnspan=3, pady=20)

file_var.trace("w", check_file)
file2_var.trace("w", check_file)

app.mainloop()