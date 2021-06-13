# atom + hydrogen
import matplotlib.pyplot as plt
import pandas as pd
import random
import os

plt.rcParams["font.size"] = "25"

titles = ["No culling", "frustum culling", "CHC KD-Tree culling", "CHC KD-Tree & frustrum culling", "Stop and Watch culling", "Stop and Watch & frustum culling"]
qttyConf = len(titles)
data = [None]*qttyConf
df = [None]*qttyConf
f, ax = plt.subplots(qttyConf, 3, figsize=(50,50))
f.tight_layout(pad=2.5)
for i in range(qttyConf):
    data[i] = pd.read_csv("./build/log/log_resources_small_museum.txt_" + str(i)+".log", delimiter=";")
    df[i] = data[i].groupby(data[i].columns.tolist(),as_index=False).size()
    df[i]["size"] = df[i]["size"]
    a = df[i].plot.scatter(x="triangles", y="fps", s="size", ax=ax[i, 0], lw=10, label="fps/triangles")
    ax[i, 0].set_xlim([0, 3030918])
    data[i].plot.line(y="fps", ax=ax[i, 1], lw=2.5, label="fps")
    data[i].plot.line(y="triangles", ax=ax[i, 2], lw=2.5, label="triangles")
    a.title.set_text(titles[i])

for i in range(qttyConf):
    print((data[i]["fps"]).mean())
    print((data[i]["triangles"]).sum())


plt.show()
plt.savefig('./doc/plot.png', dpi=300)
