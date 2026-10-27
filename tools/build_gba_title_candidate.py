#!/usr/bin/env python3
"""Create a true native-resolution, GBA-indexed Grand Anthology title mockup.
Reference art is intentionally kept separate from the current in-game title.
"""
from pathlib import Path
from PIL import Image, ImageDraw, ImageFont
import math, random

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / 'graphics' / 'branding' / 'wip'
OUT.mkdir(parents=True, exist_ok=True)
W,H = 240,160
master=Image.open(ROOT / 'graphics' / 'branding' / 'concepts' / '2026-10-26' / 'grand-anthology-logo-master.png').convert('RGBA')

# Work in RGB at exactly native GBA pixel resolution, never high-res then shrink.
im = Image.new('RGB',(W,H))
px=im.load()
for y in range(H):
    for x in range(W):
        # restrained dusk gradient; logo gets the color budget
        if y < 84:
            t=y/84
            col=(int(5+16*t), int(19+22*t),int(46+45*t))
        elif y < 115:
            t=(y-84)/31
            col=(int(22+76*t),int(40+60*t),int(91+28*t))
        elif y < 139:
            t=(y-115)/24
            col=(int(116+52*t),int(108+20*t),int(111-40*t))
        else:
            t=(y-139)/21
            col=(int(13-6*t),int(43-25*t),int(83-38*t))
        px[x,y]=col

d=ImageDraw.Draw(im)
# Sparse stars outside the logo's silhouette; no constellation clutter.
stars=[(12,11),(32,27),(17,57),(222,11),(217,36),(231,62),(64,8),(178,11),(228,82)]
for x,y in stars:
    d.point((x,y), fill=(150,170,194))
    if (x+y)%3==0: d.point((x-1,y),fill=(71,109,151))

# Horizon light: blocky soft glow with carefully limited geometry.
for r in range(27,0,-3):
    b=r/27
    color=(int(161+70*(1-b)),int(113+68*(1-b)),int(93+55*(1-b)))
    d.ellipse((120-r,127-r//3,120+r,127+r//3),fill=color)
# Layered mountains, 3-region panorama. Buildings left/Kanto, Johto pagoda center, Hoenn volcano right.
d.polygon([(0,124),(16,116),(29,121),(44,107),(61,122),(75,111),(93,125),(117,127),(139,122),(168,124),(183,109),(199,119),(210,100),(224,118),(240,113),(240,160),(0,160)],fill=(37,62,99))
d.polygon([(0,131),(24,125),(37,129),(55,119),(74,130),(93,128),(110,133),(134,127),(145,130),(158,127),(179,116),(202,125),(217,119),(240,128),(240,160),(0,160)],fill=(20,46,79))
# Hoenn mountain highlight line, restrained.
d.line([(199,119),(210,100),(222,116)],fill=(88,105,134),width=1)
# Soft smoke/cloud line behind summit
d.line([(208,98),(211,94),(216,95),(220,92)],fill=(97,102,143),width=1)

# Kanto waterside village: four rooftops instead of over-detailed city skyline.
d.polygon([(0,135),(9,129),(20,133),(29,126),(38,130),(48,125),(58,130),(78,134),(82,160),(0,160)],fill=(9,33,62))
for cx,cy,ww in [(10,124,11),(30,124,12),(49,121,12)]:
    d.rectangle((cx,cy,cx+ww,cy+12), fill=(12,34,64))
    d.polygon([(cx-2,cy),(cx+ww//2,cy-6),(cx+ww+2,cy)],fill=(10,30,56))
for x,y in [(14,129),(34,129),(53,126)]:d.rectangle((x,y,x+1,y+2),fill=(184,143,89))

# Johto: single iconic tower, not an intricate shrine.
mx=119
d.rectangle((mx-4,111,mx+4,137),fill=(10,30,56))
for yy,ww in [(106,21),(112,18),(119,15),(126,12)]:
    d.polygon([(mx-ww//2,yy+3),(mx,yy-1),(mx+ww//2,yy+3)],fill=(7,24,47))
    d.line([(mx-ww//2,yy+3),(mx+ww//2,yy+3)],fill=(67,81,112))
d.line([(mx,101),(mx,105)],fill=(15,28,51))

# Calm water: broad band and a few horizontal glints, rather than noisy waves.
d.rectangle((0,138,W,159),fill=(10,37,72))
for x1,x2,y,c in [(15,54,141,(27,65,99)),(71,100,146,(25,67,101)),(142,186,143,(25,68,106)),(197,230,150,(31,71,103)),(88,130,137,(120,112,112))]:
    d.line((x1,y,x2,y),fill=c)

# Bold standalone approved project logo, natively 216x72 with alpha compositing.
logo=master.resize((216,72),Image.Resampling.LANCZOS)
im.paste(logo,(12,9),logo)

# Bottom prompt in hand-clean 1x text, not squeezed out of concept art.
# Use the Pillow 8px bitmap font, then a pixel shadow and inset backplate.
d=ImageDraw.Draw(im)
d.rounded_rectangle((71,141,169,155),radius=2,fill=(7,21,45),outline=(82,97,130),width=1)
font=ImageFont.load_default()
text='PRESS START'
bb=d.textbbox((0,0),text,font=font)
tw=bb[2]-bb[0]
x=(W-tw)//2
# offset accounts for default bitmap glyph top bearing
d.text((x+1,140+1),text,font=font,fill=(3,9,27))
d.text((x,140),text,font=font,fill=(248,234,190))

# RGB555 rounding before quantization replicates actual GBA channel resolution.
quantized_rgb=im.point(lambda v:(v//8)*8)
q=quantized_rgb.quantize(colors=224,method=Image.Quantize.MEDIANCUT,dither=Image.Dither.NONE)
# Force all real pixels to start at index 1; index zero reserved by conversion script.
data=bytes(v+1 for v in q.get_flattened_data())
indexed=Image.frombytes('P',(W,H),data)
pal=q.getpalette()[:224*3]
indexed.putpalette([0,0,0]+pal + [0,0,0]*(256-225))
indexed.save(OUT/'grand_anthology_title_gba_candidate_240x160.png',optimize=True)
im.resize((960,640),Image.Resampling.NEAREST).save(OUT/'grand_anthology_title_gba_candidate_4x.png')
indexed.convert('RGB').resize((960,640),Image.Resampling.NEAREST).save(OUT/'grand_anthology_title_gba_indexed_4x.png')
print('Native:', indexed.size,indexed.mode,'actual indices:',len(set(indexed.get_flattened_data())), 'max:', max(indexed.get_flattened_data()))
print('Files:',*(str(p)+':'+str(p.stat().st_size) for p in [OUT/'grand_anthology_title_gba_candidate_240x160.png',OUT/'grand_anthology_title_gba_indexed_4x.png']))