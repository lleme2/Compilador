def gerar_obj(arquivo_codigo, arquivo_obj):
    with open(arquivo_codigo, 'r') as f:
        codigo = f.read().splitlines()

    with open(arquivo_obj, 'wb') as f_obj:
        for linha in codigo:
            linha = linha.strip()  # Remove espaços extras
            if linha:  # Ignora linhas vazias
                # Codifica a string para bytes e grava no arquivo
                f_obj.write(linha.encode('utf-8'))  # Usando UTF-8 para codificação
                f_obj.write(b'\n')  # Adiciona uma nova linha entre as instruções (se necessário)

# Usando a função
gerar_obj('d:/Users/Home/Documents/GitHub/demo/Compilador/gera.txt', 'd:/Users/Home/Documents/GitHub/demo/Compilador/object.obj')