# To build(save changes) run "cmake --build . --config Release"
# Run "python gui.py" in this folder to run the program

import subprocess
import os
import tkinter as tk
from tkinter import messagebox, filedialog

# must match tsp.cpp cityList
CITIES = [
    "Delhi","Mumbai","Hong Kong","Tokyo","Bangkok","Singapore",
    "Sydney (Australia)","Melbourne (Australia)","Dubai","Cairo",
    "Moscow","Stockholm","Seoul","Canada","San Francisco (USA)",
    "Washington DC (USA)","LA (USA)","Chicago (USA)","New York (USA)",
    "Cape Town (South Africa)","Rio (Brazil)","Santiago","Mexico City",
    "Peru","Miami (USA)","London","Berlin (Germany)","Rome","Paris",
    "Spain","Havana"
]

class TravelPlannerApp:
    def __init__(self, root):
        root.title("Travel Planner")
        root.geometry("600x500")

        default_exe = os.path.normpath(
            os.path.join(os.path.dirname(__file__), '../cpp/build/Release/tsp.exe')
        )
        tk.Label(root, text="TSP Executable Path:").pack(anchor="w", padx=10, pady=(10,0))
        self.exe_var = tk.StringVar(value=default_exe)
        frame = tk.Frame(root)
        frame.pack(fill="x", padx=10)
        tk.Entry(frame, textvariable=self.exe_var).pack(side="left", fill="x", expand=True)
        tk.Button(frame, text="Browse", command=self.browse).pack(side="right", padx=5)

        tk.Label(root, text="Start City:").pack(anchor="w", padx=10, pady=(10,0))
        self.start_var = tk.StringVar(value=CITIES[0])
        tk.OptionMenu(root, self.start_var, *CITIES).pack(fill="x", padx=10)

        tk.Label(root, text="Select Destinations:").pack(anchor="w", padx=10, pady=(10,0))
        self.listbox = tk.Listbox(root, selectmode="multiple", height=12)
        for city in CITIES:
            self.listbox.insert(tk.END, city)
        self.listbox.pack(fill="both", expand=True, padx=10)

        tk.Button(root, text="Plan Trip", command=self.plan).pack(pady=10)
        self.output = tk.Text(root, height=8)
        self.output.pack(fill="x", padx=10, pady=(0,10))

    def browse(self):
        path = filedialog.askopenfilename(
            title="Select tsp.exe",
            filetypes=[("Executable","*.exe"), ("All files","*.*")]
        )
        if path:
            self.exe_var.set(path)

    def plan(self):
        exe = self.exe_var.get()
        if not os.path.isfile(exe):
            messagebox.showerror("Error","Executable not found:\n" + exe)
            return

        start = self.start_var.get()
        dests = [self.listbox.get(i) for i in self.listbox.curselection() if self.listbox.get(i)!=start]
        if not dests:
            messagebox.showwarning("No destinations","Select at least one destination.")
            return

        cmd = [exe, start] + dests
        try:
            out = subprocess.check_output(cmd, stderr=subprocess.STDOUT, text=True)
        except subprocess.CalledProcessError as e:
            out = "Error:\n" + e.output
        except Exception as e:
            out = f"Unexpected error:\n{e}"

        self.output.delete(1.0, tk.END)
        self.output.insert(tk.END, out)

if __name__ == "__main__":
    root = tk.Tk()
    TravelPlannerApp(root)
    root.mainloop()
