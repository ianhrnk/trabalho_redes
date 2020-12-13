O trabalho foi desenvolvido em C/C++ e foi particionado em três pastas principais:
servidor, cliente e src.

Na pasta src estão os códigos fonte do cliente e do servidor. As outras duas pastas foram utilizadas
para representar os ambientes do cliente e do servidor. Os comandos para as funcionalidades requisitadas
são:
    Criar diretório - make_dir <nome_do_diretório>
    Remover diretório - remove_dir <nome_do_diretório>
    Listar conteúdo do diretório - ls <nome_do_diretório> (o argumento é opcional)
    Enviar arquivo - send_file <nome_do_arquivo>
    Remover arquivo - remove_file <nome_do_arquivo>

Observações:
    - O arquivo a ser enviado deve estar na pasta cliente;
    - O envio do arquivo pode demorar conforme o tamanho dele;
    - É possível listar arquivos do diretório principal ou também de um diretório existente.