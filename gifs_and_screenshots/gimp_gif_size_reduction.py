image = gimp.image_list()[0]
for (index,layer) in enumerate(image.layers):
  if not index%3:
    image.remove_layer(layer)