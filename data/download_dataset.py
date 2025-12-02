#!/usr/bin/env python3
"""
Script for downloading and preparing network traffic dataset
"""

import pandas as pd
import numpy as np
import requests
import zipfile
import os
from sklearn.model_selection import train_test_split

def download_kdd_dataset():
    """Download KDD Cup 1999 dataset for network intrusion detection"""
    
    print("Downloading KDD Cup 1999 dataset...")
    
    # URLs для скачивания
    url = "http://kdd.ics.uci.edu/databases/kddcup99/kddcup.data_10_percent.gz"
    
    # Создание директории данных
    os.makedirs("data", exist_ok=True)
    
    # Загрузка файла
    filename = "data/kddcup.data_10_percent.gz"
    
    if not os.path.exists(filename):
        print(f"Downloading from {url}...")
        response = requests.get(url, stream=True)
        with open(filename, 'wb') as f:
            for chunk in response.iter_content(chunk_size=8192):
                f.write(chunk)
        print("Download complete.")
    else:
        print("Dataset already exists.")
    
    return filename

def prepare_dataset():
    """Prepare and preprocess the dataset"""
    
    print("\nPreparing dataset...")
    
    # Загрузка данных
    # Для демонстрации создадим синтетические данные
    # В реальном проекте здесь будет загрузка реального dataset
    
    np.random.seed(42)
    n_samples = 5000
    
    # Создание синтетических признаков сетевого трафика
    data = {
        'duration': np.random.exponential(1.0, n_samples),
        'protocol_type': np.random.choice([0, 1, 2], n_samples),
        'service': np.random.choice(range(70), n_samples),
        'flag': np.random.choice(range(11), n_samples),
        'src_bytes': np.random.lognormal(7, 2, n_samples),
        'dst_bytes': np.random.lognormal(7, 2, n_samples),
        'land': np.random.choice([0, 1], n_samples),
        'wrong_fragment': np.random.poisson(0.1, n_samples),
        'urgent': np.random.poisson(0.01, n_samples),
        'hot': np.random.poisson(0.5, n_samples),
        'num_failed_logins': np.random.poisson(0.1, n_samples),
        'logged_in': np.random.choice([0, 1], n_samples),
        'num_compromised': np.random.poisson(0.05, n_samples),
        'root_shell': np.random.choice([0, 1], n_samples, p=[0.99, 0.01]),
        'su_attempted': np.random.choice([0, 1], n_samples),
        'num_root': np.random.poisson(0.1, n_samples),
        'num_file_creations': np.random.poisson(0.2, n_samples),
        'num_shells': np.random.poisson(0.01, n_samples),
        'num_access_files': np.random.poisson(0.1, n_samples),
        'num_outbound_cmds': np.zeros(n_samples),
        'is_host_login': np.random.choice([0, 1], n_samples),
        'is_guest_login': np.random.choice([0, 1], n_samples),
        'count': np.random.poisson(100, n_samples),
        'srv_count': np.random.poisson(100, n_samples),
        'serror_rate': np.random.uniform(0, 1, n_samples),
        'srv_serror_rate': np.random.uniform(0, 1, n_samples),
        'rerror_rate': np.random.uniform(0, 1, n_samples),
        'srv_rerror_rate': np.random.uniform(0, 1, n_samples),
        'same_srv_rate': np.random.uniform(0, 1, n_samples),
        'diff_srv_rate': np.random.uniform(0, 1, n_samples),
        'srv_diff_host_rate': np.random.uniform(0, 1, n_samples),
        'dst_host_count': np.random.poisson(255, n_samples),
        'dst_host_srv_count': np.random.poisson(255, n_samples),
        'dst_host_same_srv_rate': np.random.uniform(0, 1, n_samples),
        'dst_host_diff_srv_rate': np.random.uniform(0, 1, n_samples),
        'dst_host_same_src_port_rate': np.random.uniform(0, 1, n_samples),
        'dst_host_srv_diff_host_rate': np.random.uniform(0, 1, n_samples),
        'dst_host_serror_rate': np.random.uniform(0, 1, n_samples),
        'dst_host_srv_serror_rate': np.random.uniform(0, 1, n_samples),
        'dst_host_rerror_rate': np.random.uniform(0, 1, n_samples),
        'dst_host_srv_rerror_rate': np.random.uniform(0, 1, n_samples)
    }
    
    # Создание DataFrame
    df = pd.DataFrame(data)
    
    # Генерация меток (нормальный трафик vs атака)
    attack_prob = 0.3  # 30% атак
    attack_types = ['normal', 'dos', 'probe', 'r2l', 'u2r']
    
    labels = []
    for i in range(n_samples):
        if np.random.random() < attack_prob:
            label = np.random.choice(attack_types[1:])  # Выбор типа атаки
        else:
            label = 'normal'
        labels.append(label)
    
    df['label'] = labels
    
    # Сохранение в CSV
    output_file = "data/network_traffic.csv"
    df.to_csv(output_file, index=False)
    
    print(f"Dataset saved to {output_file}")
    print(f"Shape: {df.shape}")
    print(f"Labels distribution:\n{df['label'].value_counts()}")
    
    return output_file

def generate_test_data():
    """Generate test data for C++ program"""
    
    print("\nGenerating test data for C++...")
    
    # Создание небольшого тестового набора
    test_data = []
    n_test = 100
    
    for i in range(n_test):
        # Создание случайного сетевого пакета
        packet_size = np.random.randint(64, 1500)
        packet = np.random.bytes(packet_size)
        
        # Сохранение в формате, понятном для C++
        hex_string = packet.hex()
        test_data.append(f"{packet_size}:{hex_string}")
    
    # Сохранение тестовых данных
    with open("data/test_packets.txt", "w") as f:
        for line in test_data:
            f.write(line + "\n")
    
    print(f"Generated {n_test} test packets")

if __name__ == "__main__":
    print("Network Traffic Dataset Preparation")
    print("=" * 50)
    
    # Создание синтетического dataset
    dataset_file = prepare_dataset()
    
    # Генерация тестовых данных
    generate_test_data()
    
    print("\nDataset preparation complete!")
    print(f"Dataset file: {dataset_file}")
    print("Test packets: data/test_packets.txt")