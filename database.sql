-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Máy chủ: 127.0.0.1
-- Thời gian đã tạo: Th12 30, 2023 lúc 09:57 AM
-- Phiên bản máy phục vụ: 10.4.28-MariaDB
-- Phiên bản PHP: 8.2.4

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Cơ sở dữ liệu: `database_iot`
--

-- --------------------------------------------------------

--
-- Cấu trúc bảng cho bảng `sensor`
--

CREATE TABLE `sensor` (
  `id` int(11) NOT NULL,
  `name` varchar(255) DEFAULT NULL,
  `location` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Đang đổ dữ liệu cho bảng `sensor`
--

INSERT INTO `sensor` (`id`, `name`, `location`) VALUES
(1, 'esp8266', 'location1'),
(2, 'esp8266', 'location2'),
(3, 'esp8266', 'location3'),
(4, 'esp8266', 'location4');

-- --------------------------------------------------------

--
-- Cấu trúc bảng cho bảng `thongso`
--

CREATE TABLE `thongso` (
  `sensor_id` int(11) DEFAULT NULL,
  `nhietdo` float DEFAULT NULL,
  `doam` float DEFAULT NULL,
  `doamdat` float DEFAULT NULL,
  `time` time DEFAULT NULL,
  `date` date DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Chỉ mục cho các bảng đã đổ
--

--
-- Chỉ mục cho bảng `sensor`
--
ALTER TABLE `sensor`
  ADD PRIMARY KEY (`id`);

--
-- Chỉ mục cho bảng `thongso`
--
ALTER TABLE `thongso`
  ADD KEY `sensor_id` (`sensor_id`);

--
-- Các ràng buộc cho các bảng đã đổ
--

--
-- Các ràng buộc cho bảng `thongso`
--
ALTER TABLE `thongso`
  ADD CONSTRAINT `thongso_ibfk_1` FOREIGN KEY (`sensor_id`) REFERENCES `sensor` (`id`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;