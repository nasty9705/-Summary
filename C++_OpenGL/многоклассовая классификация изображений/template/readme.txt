Турлапова Анастасия Сергеевна, 314 группа
Выполнено: базовая часть и цветовые признаки.
Написан и использован класс UnaryMatrixOperator.
В файле classifier.h изменена 25, 33, 43 строчки:
shared_ptr<struct model> model_;
model_ = shared_ptr<struct model>(model);
model_ = shared_ptr<struct model>(load_model(model_file.c_str()));

