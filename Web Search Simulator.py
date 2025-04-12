import requests
import re
import xml.etree.ElementTree as ET

# Le damos la bienvenida al usuario y pedimos la noticia a buscar
print('Hola bienvenido al procesador de feeds de Google news, por favor introduce la noticia que deseas ver')

news_to_search = input('Por favor ingrese lo que quiere buscar: ')

# Conseguimos acceso al XML a través del módulo requests
url = 'https://news.google.com/rss?hl=es-419&gl=CO&ceid=CO:es-419'
feed = requests.get(url)

# Esta función permitirá parsear el archivo XML y devolver las noticias en diccionarios
def parse_rss(feed):
    # Declaramos nuestras principales variables y parseamos el string en un árbol
    tree = ET.ElementTree(ET.fromstring(feed.content))
    root = tree.getroot()
    list_of_news = []

    # Accedemos al canal y buscamos la cantidad de 'items' que haya
    channel = root.find('channel')

    # Iterar sobre los elementos 'item' dentro de 'channel'
    for item in channel.findall('item'):
        title = item.find('title').text
        description = item.find('description').text

        # Generamos la lista de diccionarios
        noticia = {'title': title, 'Description': description}
        list_of_news.append(noticia)

    # Mostrar las noticias parseadas al final
    return list_of_news

# Implementación de disparadores que ayudan a categorizar las noticias
list_of_news = parse_rss(feed)

def search_tittle(news={'title':""}):
    #Conseguimos el título a partir del diccionario y generamos la expreción regular 
    title = news['title']
    patron = fr"\b {news_to_search}\b"
    if re.search(patron, title, re.IGNORECASE):
      return True
    else:
       return False
    
def search_description(news={'Description':""}):
    #Conseguimos la descripción a partir del diccionario
    description = news['Description']
    patron = fr"\b{news_to_search}\b"
    if re.search(patron, description, re.IGNORECASE):
      return True
    else:
       return False

#Generamos una lista con las funciones diparadoras estas seran referencias para no llamar a las funciones inmediatamente
triggers = [search_tittle, search_description]

#Filtramos las noticias que al menos activen un disparador
def filter_news(news_list, triggers):
    filtered_news = []
    for dic in news_list:
        for trigger in triggers:
            # Aplica cada función trigger con los argumentos correspondientes
            if trigger(dic):
                filtered_news.append(dic)
                break # Rompemos el ciclo para no agregar dos veces una misma noticia
    return filtered_news

filtered_news = filter_news(list_of_news, triggers)

# Verificar si hay noticias filtradas
if filtered_news:
    print(f"Se encontraron {len(filtered_news)} noticias.")
else:
    print("No se encontraron noticias que coincidan con la búsqueda.")

def display_alerts(filtered_news, filename='Noticias_encontradas.txt'):
    with open(filename, 'w', encoding='utf-8') as archivo:  # 'utf-8' para manejar caracteres especiales
        for noticia in filtered_news:
            # Escribir el título y descripción de cada noticia en el archivo
            archivo.write(f"Título: {noticia['title']}\n")
            archivo.write(f"Descripción: {noticia['Description']}\n")
            archivo.write("\n" + "-"*40 + "\n\n")  # Separador entre noticias

display_alerts(filtered_news,)