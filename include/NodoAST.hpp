#pragma once

#include <memory>
#include <string>
#include <vector>

class NodoAST : public std::enable_shared_from_this<NodoAST> {
 private:
  std::string tipo;
  std::string valor;
  std::vector<std::shared_ptr<NodoAST>> hijos;
  std::weak_ptr<NodoAST> padre;  // weak_ptr para evitar ciclos de referencia

 public:
  NodoAST(const std::string& tipo, const std::string& valor = "")
      : tipo(tipo), valor(valor) {
  }

  // Getters
  const std::string& getTipo() const {
    return tipo;
  }
  const std::string& getValor() const {
    return valor;
  }
  const std::vector<std::shared_ptr<NodoAST>>& getHijos() const {
    return hijos;
  }
  std::shared_ptr<NodoAST> getParent() const {
    return padre.lock();
  }

  // Agregar hijo
  void agregarHijo(std::shared_ptr<NodoAST> hijo) {
    hijo->padre = shared_from_this();
    hijos.push_back(hijo);
  }

  // Crear y agregar hijo en un solo paso
  std::shared_ptr<NodoAST> crearHijo(const std::string& tipo,
                                     const std::string& valor = "") {
    auto hijo = std::make_shared<NodoAST>(tipo, valor);
    hijo->padre = shared_from_this();
    hijos.push_back(hijo);
    return hijo;
  }
};
