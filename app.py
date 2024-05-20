import streamlit as st
import base64
import pyautogui
import numpy as np
import pandas as pd 
#Função Main
def main():
    st.set_page_config(page_title="GamaCubeDesign - Telemetria e Telecomando - Gama-Sat",page_icon="gama_logo.png")
    st.markdown(
        """
        <div style="text-align: center;">
            <img src="data:image/png;base64,{}" alt="Gama Logo" style="width: 200px;margin-bottom: 10px;">
        </div>
        """.format(get_base64_encoded_image("gama_logo.png")),
        unsafe_allow_html=True
    ) #Centralizando o Logo da Gama

    st.markdown(
        """
        <div style="text-align: center; background-color: #008000; padding: 1px; border-radius: 10px;width: 500px;margin-bottom: 40px;margin: auto;">
            <h1 style="text-align: center; color: #f0f0f0;">Gama-Sat</h1>
        </div>
        """,
        unsafe_allow_html=True
    )
    baud_rate = st.selectbox("Baud Rate", ["9600", "19200", "38400", "57600", "115200"])
    baud_rate = st.selectbox("Porta", ["COM3", "COM4", "COM11"])
    col1, col2, col3, col4, col5, col6 = st.columns(6)
    st.markdown(
        """
        <style>
        .stButton > button {
            width: 115px;
        }
        </style>
        """,
        unsafe_allow_html=True
    )
    bt_iniciar=st.button("iniciar",type="primary")
    

    
    with col1:
        if st.button("Saúde"):
            # Lógica para lidar com a ação do botão "Saúde"
            st.write("Botão Saúde clicado!")
            #desenhando os graficos 
            chart_data = pd.DataFrame(
            np.random.randn(20, 2),
            columns=['time', 'sd memory'])
            st.line_chart(chart_data)
            #tabela de dados 
            df = pd.DataFrame(np.random.randn(50, 2), columns=("col %d" % i for i in range(2)))
        
            st.dataframe(df)  # Same as st.write(df)
        
            
    with col2:
        if st.button("Temperatura Interna"):
            # Lógica para lidar com a ação do botão "Temperatura Interna"
            st.write("Botão Temperatura Interna clicado!")
            chart_data = pd.DataFrame(
            np.random.randn(20, 1),
            columns=['temp interna'])

            st.line_chart(chart_data)
            df = pd.DataFrame(np.random.randn(50, 1), columns=("col %d" % i for i in range(1)))
        
            st.dataframe(df)  # Same as st.write(df)
        
            
            
    with col3:
        if st.button("Temperatura Externa"):
            # Lógica para lidar com a ação do botão "Temperatura Interna"
            st.write("Botão Temperatura Externa clicado!")
            chart_data = pd.DataFrame(
            np.random.randn(20, 1),
            columns=['temp externa'])

            st.line_chart(chart_data)
            df = pd.DataFrame(np.random.randn(50, 1), columns=("col %d" % i for i in range(1)))
        
            st.dataframe(df)  # Same as st.write(df)
        
            
            
    with col4:
        if st.button("Raio"):
            # Lógica para lidar com a ação do botão "Temperatura Interna"
            st.write("Botão Raio clicado!")
            chart_data = pd.DataFrame(
            np.random.randn(20, 2),
            columns=['duration', 'radius'])

            st.line_chart(chart_data)
            df = pd.DataFrame(np.random.randn(50, 2), columns=("col %d" % i for i in range(2)))
        
            st.dataframe(df)  # Same as st.write(df)
        
            
    with col5:
        if st.button("Tensão"):
            # Lógica para lidar com a ação do botão "Temperatura Interna"
            st.write("Botão tensão clicado!")
            chart_data = pd.DataFrame(
            np.random.randn(20, 1),
            columns=['battery voltage'])

            st.line_chart(chart_data)
            df = pd.DataFrame(np.random.randn(50, 1), columns=("col %d" % i for i in range(1)))
        
            st.dataframe(df)  # Same as st.write(df)
        
            
    with col6:
        if st.button("Corrente"):
            st.write("Botão Corrente clicado!")
            chart_data = pd.DataFrame(
            np.random.randn(20,3 ),
            columns=['battery current', 'battery charge','battery temp'])

            st.line_chart(chart_data)
        #tabela de dados 
            df = pd.DataFrame(np.random.randn(50, 3), columns=("col %d" % i for i in range(3)))
        
            st.dataframe(df)  # Same as st.write(df)
        
            
#Função para auxiliar na leitura da imagem através do Markdown
def get_base64_encoded_image(image_path):
    with open(image_path, "rb") as img_file:
        return base64.b64encode(img_file.read()).decode()

#Main
if __name__ == "__main__":
    main()