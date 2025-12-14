import qrcode
from qrcode.constants import ERROR_CORRECT_H

qr = qrcode.QRCode(
    version=None,
    error_correction=ERROR_CORRECT_H,
    mask_pattern=2,
    box_size=10,   # pixels per module
    border=4       # standard quiet zone
)

#data = b"\xaa" * 13
#data = "Matas Brazauskas"
data = "Matas Brazauskas"
qr.add_data(data, optimize=0)
qr.make(fit=True)

img = qr.make_image(fill_color="black", back_color="white")
img.save("qr.png")

print(qr.version);
print(qr.error_correction)
print(qr.mask_pattern)